# # https://community.platformio.org/t/changing-usb-device-vid-pid/3986/16?u=owenspencer
# Import("env")
# board_config = env.BoardConfig()
# # should be array of VID:PID pairs
# board_config.update("build.hwids", [
#     ["0x2341", "0x0243"] # VID, PID ?
# ])

Import("env")

print(env.Dump())

board_config = env.BoardConfig()
# should be array of VID:PID pairs
board_config.update("build.hwids", [
  ["0x20a0", "0x42da"]
])