defmodule KeyStore do
  @name __MODULE__
  
  def start_link do
    Agent.start_link(fn -> %{} end, name: @name)
  end

  def get(key) do
    Agent.get(@name, &Map.get(&1, key))
  end

  def put(key, value) do
    Agent.update(@name, &Map.put(&1, key, value))
  end
end
