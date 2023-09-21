import subprocess
import sys
import time
import os

def run_process_with_timeout(command, case_number,input_file, output_file, log_file, timeout_milliseconds):
    if not os.path.exists(command):
        with open(log_file, 'a') as log_stream:
            log_stream.write(f"{case_number} -1\n")
        return

    if not os.path.exists(input_file):
        with open(log_file, 'a') as log_stream:
            log_stream.write(f"{case_number} -1\n")
        return

    start_time = time.time()

    with open(input_file, 'r') as input_stream, open(output_file, 'w') as output_stream:
        process = subprocess.Popen(
            command,
            stdin=input_stream,
            stdout=output_stream,
            stderr=subprocess.PIPE,
            shell=True
        )

        while True:
            return_code = process.poll()
            if return_code is not None:
                break

            elapsed_time = (time.time() - start_time) * 1000
            if elapsed_time > timeout_milliseconds:
                process.terminate()
                with open(log_file, 'a') as log_stream:
                    log_stream.write(f"{case_number} {int(elapsed_time)}\n")
                return

            time.sleep(0.001)

        if return_code != 0:
            with open(log_file, 'a') as log_stream:
                log_stream.write(f"{case_number} -1\n")
            return
        else:
            with open(log_file, 'a') as log_stream:
                log_stream.write(f"{case_number} {int(elapsed_time)}\n")
            return


if __name__ == "__main__":
    if len(sys.argv) != 7:
        print("python3 run_command.py <cmp.exe> <case_number> <input> <output> <log> <timelimit>")
        sys.exit(1)

    command = sys.argv[1]
    case_number = int(sys.argv[2])
    input_file = sys.argv[3]
    output_file = sys.argv[4]
    log_file = sys.argv[5]
    timeout_milliseconds = float(sys.argv[6])

    run_process_with_timeout(command, case_number, input_file, output_file, log_file, timeout_milliseconds)
