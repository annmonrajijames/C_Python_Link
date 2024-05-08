import tkinter as tk
from subprocess import Popen, PIPE

def run_c_program():
    input_values = f"{a1.get()} {a2.get()} {a3.get()} {a4.get()} {a5.get()} {a6} {a7} {a8} {a9} {a10} {a11}"
    process = Popen(['./cfile'], stdin=PIPE, stdout=PIPE)
    stdout, stderr = process.communicate(input=input_values.encode())
    return stdout.decode().strip()

def submit():
    result = run_c_program()
    result_label.config(text="Output from C program: " + result)

# Tkinter window setup
root = tk.Tk()
root.title("Variable Input for C Program")

# Variables setup after creating the root window
a1, a2, a3, a4, a5 = tk.IntVar(root), tk.IntVar(root), tk.IntVar(root), tk.IntVar(root), tk.IntVar(root)
a6, a7, a8, a9, a10, a11 = 1, 22, 11, 23, 11, 99  # Static values for other variables

# Radio buttons for a1 to a5
labels = ['a1', 'a2', 'a3', 'a4', 'a5']
vars = [a1, a2, a3, a4, a5]

for i, (label, var) in enumerate(zip(labels, vars)):
    frame = tk.Frame(root)
    frame.pack(pady=5)
    tk.Label(frame, text=label).pack(side=tk.LEFT)
    tk.Radiobutton(frame, text='0', variable=var, value=0).pack(side=tk.LEFT)
    tk.Radiobutton(frame, text='1', variable=var, value=1).pack(side=tk.LEFT)
    var.set(0)  # Default value set to 0

# Submit button
submit_button = tk.Button(root, text="Run C Program", command=submit)
submit_button.pack(pady=20)

# Result label
result_label = tk.Label(root, text="Output from C program will appear here")
result_label.pack(pady=20)

root.mainloop()