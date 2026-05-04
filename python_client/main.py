import os
import json
import sys
import subprocess

def ParseArgs():
    return "--json" in sys.argv

def RunMonitor(use_json: bool) -> str | None:
    base_dir = os.path.dirname(os.path.abspath(__file__))
    monitor_path = os.path.join(base_dir, "../cpp_monitor/build/monitor")
    args = [monitor_path, "--json" if use_json else "--human"]

    try:
        proc = subprocess.run(
            args,
            capture_output = True,
            text = True,
            check=True
        )
        return proc.stdout
    except FileNotFoundError:
        print(f"Ошибка: не найден файл {monitor_path}")
    except subprocess.CalledProcessError as exp:
        print("Программа завершилась с ошибкой: ", exp)

    return None

def PrintHumanOut(data):
    print(f"CPU Usage: {data['cpu']['usage']} %")
    print(f"RAM: {data['ram']['used']} / {data['ram']['total']} MB")
    print(f"Uptime: {data['uptime']:} s")

def main():
    use_json = ParseArgs()
    output = RunMonitor(use_json)

    if output is None:
        return
    
    if use_json:
        try:
            data = json.loads(output)
            PrintHumanOut(data)
        except json.JSONDecodeError as exp:
            print("Ошибка парсинга JSON:", exp)
            print(output)
    else:
        print(output)

if __name__ == "__main__":
    main()