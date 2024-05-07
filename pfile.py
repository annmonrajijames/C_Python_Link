import subprocess

def run_c_program():
    a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11 = 1, 10, 7, 0, 6, 1, 22, 11, 23, 11, 99
    input_values = f"{a1} {a2} {a3} {a4} {a5} {a6} {a7} {a8} {a9} {a10} {a11}"
    
    # Start the compiled C program and pass the formatted string of input values
    process = subprocess.Popen(['./cfile'], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    stdout, stderr = process.communicate(input=input_values.encode())
    return stdout.decode().strip()

result = run_c_program()
print("Output from C program:", result)
