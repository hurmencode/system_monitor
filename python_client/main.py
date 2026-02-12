import os
import subprocess
import json

def RunMonitor():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    monitor_path = os.path.join(base_dir, "../cpp_monitor/build/monitor")
    try:
        proc = subprocess.run(
            [monitor_path],
            capture_output = True,
            text = True
        )

        data = json.loads(proc.stdout)
        return data
    except FileNotFoundError:
        print(f"Ошибка: не найден файл {monitor_path}")
    except json.JSONDecodeError as exp:
        print(f"Ошибка парсинга JSON: ", exp)
        print(f"Вывод C++ программы:")
        print(proc.stdout)
    except subprocess.CalledProcessError as exp:
        print("Программа завершилась с ошибкой: ", exp)

    return None

def PrintHumanOut(data):
    if not data:
        print("Нет данных для вывода")
        return
    
    cpu_usage = data["cpu"]["usage"]
    ram_used = data["ram"]["used"]
    ram_total = data["ram"]["total"]
    uptime = data["uptime"]

    print(f"CPU Usage: {cpu_usage} %")
    print(f"RAM: {ram_used} / {ram_total} MB")
    print(f"Uptime: {uptime} s")

def main():
    data = RunMonitor()
    PrintHumanOut(data)

if __name__ == "__main__":
    main()