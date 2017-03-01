#!/usr/bin/env ruby

require 'socket'
require 'optparse'

#/ PURPOSE:
#/ The script is a PoC to demonstrate how CONNECT-tunneling via HTTP-Proxy works
#/ Your HTTP Proxy must support and allow CONNECT to non 443 ports
#/ USAGE:
#/ ./script.rb --dst-host example.com --dst-port 22 --proxy-host localhost --proxy-port 3128

# CONNECT-tunnelling PoC class
class TcpProxy
  def initialize(dst_host, dst_port, http_proxy_host, http_proxy_port)
    @dst_host = dst_host
    @dst_port = dst_port
    @http_proxy_host = http_proxy_host
    @http_proxy_port = http_proxy_port
    @local_host = 'localhost'
    @local_port = 2201
    @block_size = 1024

    Thread.abort_on_exception = true
  end

  def run
    init_server
    loop do
      Thread.start(@server.accept) { |local| conn_thread(local) }
    end
  end

  def init_server
    @server = TCPServer.open(@local_host, @local_port)
    port = @server.addr[1]
    addrs = @server.addr[2..-1].uniq.collect { |a| "#{a}:#{port}" }.join(', ')
    puts "*** listening on #{addrs}"
  end

  def init_tunnel(remote)
    remote.write("CONNECT #{@dst_host}:#{@dst_port} HTTP/1.1\r\n\r\n")
    select([remote], nil, nil)
    data = remote.recv(@block_size)
    unless data.start_with?('HTTP/1.1 2')
      puts "HTTP Proxy returned error status: #{data}"
      exit
    end
  end

  def conn_thread(local)
    port, name = local.peeraddr[1..2]
    puts "*** receiving from #{name}:#{port}"

    remote = TCPSocket.new(@http_proxy_host, @http_proxy_port)

    init_tunnel remote

    loop do
      ready = select([local, remote], nil, nil)
      if ready[0].include? local
        # local -> remote
        data = local.recv(@block_size)
        if data.empty?
          puts 'local end closed connection'
          break
        end
        remote.write(data)
      end
      if ready[0].include? remote
        # remote -> local
        data = remote.recv(@block_size)
        if data.empty?
          puts 'remote end closed connection'
          break
        end
        local.write(data)
      end
    end

    local.close
    remote.close

    puts "*** done with #{name}:#{port}"
  end
end

dst_host = ''
dst_port = 0
proxy_host = 'localhost'
proxy_port = 3128

ARGV.options do |opts|
  opts.on('--dst-host=val', String)   { |val| dst_host = val }
  opts.on('--dst-port=val', Integer)  { |val| dst_port = val }
  opts.on('--proxy-host=val', String)   { |val| proxy_host = val }
  opts.on('--proxy-port=val', Integer)  { |val| proxy_port = val }
  opts.on_tail('-h', '--help') { exec "grep ^#/ #{__FILE__}"; exit }
  opts.parse!
end

TcpProxy.new(dst_host, dst_port, proxy_host, proxy_port).run

__END__
