defmodule ApiWrapper.Session do
  @moduledoc """
  Manage authentication
  """

  @doc """
  Retrieve session
  """
  def start() do
    timestamp = :os.system_time(:seconds)
    nonce = :rand.uniform(100000)
    
    signature = :crypto.hmac(:sha, ApiWrapper.get_env(:auth_secret),
      "application_id=#{ApiWrapper.get_env(:application_id)}" <>
        "&auth_key=#{ApiWrapper.get_env(:auth_key)}" <>
        "&nonce=#{nonce}&timestamp=#{timestamp}")
    |> Base.encode16
    |> String.downcase

    result = ApiWrapper.post! "session",
      JSX.encode!(
        application_id: ApiWrapper.get_env(:application_id),
        auth_key: ApiWrapper.get_env(:auth_key),
        timestamp: timestamp,
        nonce: nonce,
        signature: signature ),
      [{"Content-Type", "application/json"}, {"QuickBlox-REST-API-Version", "0.1.0"}]
    KeyStore.put "session_token", result.body["session"]["token"]
  end

  @doc """
  Invalidate session
  """
  def destroy() do
    token = KeyStore.get "session_token"
    ApiWrapper.delete "session", [{"QuickBlox-REST-API-Version", "0.1.0"}, {"QB-Token", token}]
  end
end
