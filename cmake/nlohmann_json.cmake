if(NOT TARGET nlohmann_json::nlohmann_json)
  message("[ArmorerPhys] Fetching nlohmann_json")
  FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
  FetchContent_MakeAvailable(json)
endif()
