import subprocess
import json

proc = subprocess.run(
    ["../cpp_monitor/build/monitor"],
    capture_output = True,
    text = True
)

print(proc.stdout)