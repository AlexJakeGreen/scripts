defmodule ApiWrapper.Mixfile do
  use Mix.Project

  def project do
    [app: :api_wrapper,
     version: "0.1.0",
     elixir: "~> 1.3",
     build_embedded: Mix.env == :prod,
     start_permanent: Mix.env == :prod,
     deps: deps()]
  end

  # Configuration for the OTP application
  #
  # Type "mix help compile.app" for more information
  def application do
    [applications: [:logger, :httpoison],
     mod: {ApiWrapper, []}]
  end

  # Dependencies can be Hex packages:
  #
  #   {:mydep, "~> 0.3.0"}
  #
  # Or git/path repositories:
  #
  #   {:mydep, git: "https://github.com/elixir-lang/mydep.git", tag: "0.1.0"}
  #
  # Type "mix help deps" for more examples and options
  defp deps do
    [
      { :httpoison, "~> 0.9.0" },
      { :exjsx, "~> 3.2.0", app: false },
      { :ex_doc, "~> 0.6.1", only: :docs },
      { :earmark, "~> 0.1.12", only: :docs }
    ]
  end
end
