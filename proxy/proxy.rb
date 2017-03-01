#!/usr/bin/env ruby

require 'socket'
require 'uri'
require 'openssl'

class Proxy
  
  def run port
    begin
      @socket = TCPServer.new('127.0.0.1', port) 
      sslContext = OpenSSL::SSL::SSLContext.new
      sslContext.cert = OpenSSL::X509::Certificate.new(File.open("mitm.crt"))
      sslContext.key = OpenSSL::PKey::RSA.new(File.open("mitm.key"))
      sslServer = OpenSSL::SSL::SSLServer.new(@socket, sslContext)
      sslServer.start_immediately = false
      loop do
        s = sslServer.accept
        Thread.new s, &method(:handle_request)
      end
    rescue Interrupt
      puts 'Got Interrupt..'
    ensure
      @socket.close if @socket
      puts 'Quitting.'
    end
  end

  
  def handle_request to_client
    request_line = to_client.readline
    puts "===(me): parsing #{request_line}"

    # GET http://jee.org.ua/ HTTP/1.1
    # CONNECT jee.org.ua:443 HTTP/1.1
    method  = request_line[/^\w+/]
    url     = request_line[/^\w+\s+(\S+)/, 1]

    if ! url.start_with?('http://', 'https://')
      if /^[^\/]+:443\/?/.match(url)
        url = 'https://' + url + '/'
      else
        url = 'http://' + url
      end
    end

    version = request_line[/HTTP\/(1\.\d)\s*$/, 1]
    uri     = URI::parse url
    puts "-->(me): #{request_line}"
    puts "===(me): method: #{method}, url:  #{url}, version: #{version}"

    if method == 'CONNECT'
      puts '===(me): CONNECT called. Upgrading to SSL...'
      to_client.write("HTTP/1.0 200 Connection established\r\nProxy-agent: ptox/0.0.0\r\n\r\n")
      puts "-->(client): HTTP/1.0 200 Connection established"
      to_client.accept
      puts '===(me): Accepted ssl...'
    end

    
    #to_server = TCPSocket.new(uri.host, (uri.port.nil? ? 80 : uri.port))
    dest_port = (uri.port.nil? ? 80 : uri.port)
    puts "===(me): Opening a socket to #{uri.scheme}://#{uri.host}:#{dest_port}"
    to_server = TCPSocket.new(uri.host, dest_port)
    if uri.scheme == 'https'
      puts '===(me): Upgrading to SSL...'
      ctx = OpenSSL::SSL::SSLContext.new
      #ctx.set_params(verify_mode: OpenSSL::SSL::VERIFY_PEER)
      #to_server = OpenSSL::SSL::SSLSocket.new(to_server, ctx)
      to_server = OpenSSL::SSL::SSLSocket.new(to_server, ctx).tap do |socket|
        socket.sync_close = true
        socket.connect
      end
      
      puts '===(me): SSL started'
    end
    #request = "GET #{uri.path}/?#{uri.query} HTTP/#{version}\r\n"
    request = "GET #{uri.path}?#{uri.query} HTTP/#{version}\r\n"
    puts "-->(#{uri.host}): #{request}"
    to_server.write(request)
    content_len = 0
    
    loop do      
      line = to_client.readline
      
      if line =~ /^Content-Length:\s+(\d+)\s*$/
        content_len = $1.to_i
      end
      
      # Strip proxy headers
      if line =~ /^proxy/i
        next
      elsif line.strip.empty?
        puts "-->(#{uri.host}):" + ' Connection: close\r\n\r\n'
        to_server.write("Connection: close\r\n\r\n")
        
        if content_len > 0
          puts "-->(#{uri.host}): sent body #{content_len} bytes"
          to_server.write(to_client.read(content_len))
        end
        
        break
      else
        puts "-->(#{uri.host}): #{line}"
        to_server.write(line)
      end
    end
    
    buff = ""
    loop do
      to_server.read(2048, buff)
      to_client.write(buff)
      break if buff.size <= 0
    end
    
    # Close the sockets
    to_client.close
    to_server.close
  end
  
end


Proxy.new.run 8008
