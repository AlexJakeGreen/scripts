defmodule ApiWrapper do
  @moduledoc """
  An HTTP client
  """

  use Application
  use HTTPoison.Base

  # See http://elixir-lang.org/docs/stable/elixir/Application.html
  # for more information on OTP Applications
  def start(_type, _args) do
    import Supervisor.Spec, warn: false

    # Define workers and child supervisors to be supervised
    children = [
      # Starts a worker by calling: ApiWrapper.Worker.start_link(arg1, arg2, arg3)
      # worker(ApiWrapper.Worker, [arg1, arg2, arg3]),
      worker(KeyStore, [])
    ]
    # See http://elixir-lang.org/docs/stable/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: ApiWrapper.Supervisor]
    Supervisor.start_link(children, opts)
  end

  @doc """
  Creates the URL for our endpoint
  Args:
    * endpoint - part of the API we're hitting
  """
  def process_url(endpoint) do
    IO.puts "https://api.quickblox.com/" <> endpoint <> ".json"
    "http://api.quickblox.com/" <> endpoint <> ".json"
  end

  @doc """
  Converts the binary keys in our response to strings.
  Args:
    * body - string binary response
  Returns Record or ArgumentError
  """
  def process_response_body(body) do
    JSX.decode!(body)
  end

  @doc """
  Gets the api key from :api_wrapper, :key application env
  """
  def get_env(key) do
    Application.fetch_env!(:api_wrapper, key)
  end
end
