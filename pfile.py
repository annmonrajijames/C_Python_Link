import tkinter as tk
from subprocess import Popen, PIPE

def run_c_program():
    input_values = f"{a1.get()} {a2.get()} {a3.get()} {a4.get()} {a5.get()} {a6.get()} {a7.get()} {a8.get()} {a9.get()} {a10.get()} {a11.get()}"
    process = Popen(['./cfile'], stdin=PIPE, stdout=PIPE)
    stdout, stderr = process.communicate(input=input_values.encode())
    result_label.config(text="Output from C program: " + stdout.decode().strip())

# Tkinter window setup
root = tk.Tk()
root.title("Variable Input for C Program")

# Variables setup after creating the root window
a1, a2, a3, a4, a5 = tk.IntVar(root), tk.IntVar(root), tk.IntVar(root), tk.IntVar(root), tk.IntVar(root)
a6, a7, a8, a9, a10, a11 = tk.IntVar(root), tk.IntVar(root), tk.IntVar(root), tk.IntVar(root), tk.IntVar(root), tk.IntVar(root)

def create_radio_frame(parent, label, var):
    frame = tk.Frame(parent)
    tk.Label(frame, text=label).pack(side=tk.LEFT)
    tk.Radiobutton(frame, text='0', variable=var, value=0, command=run_c_program).pack(side=tk.LEFT)
    tk.Radiobutton(frame, text='1', variable=var, value=1, command=run_c_program).pack(side=tk.LEFT)
    var.set(0)
    return frame

def create_scale_frame(parent, label, var):
    frame = tk.Frame(parent)
    tk.Label(frame, text=label).pack(side=tk.LEFT)
    scale = tk.Scale(frame, from_=0, to=100, orient='horizontal', variable=var, command=lambda v: run_c_program())
    scale.pack(fill='x', expand=True)
    var.set(50)
    return frame

# Create radio button frames for a1 to a5
labels = ['a1', 'a2', 'a3', 'a4', 'a5']
vars = [a1, a2, a3, a4, a5]
for label, var in zip(labels, vars):
    frame = create_radio_frame(root, label, var)
    frame.pack(pady=5)

# Create scale frames for a6 to a11
scale_labels = ['a6', 'a7', 'a8', 'a9', 'a10', 'a11']
scale_vars = [a6, a7, a8, a9, a10, a11]
for label, var in zip(scale_labels, scale_vars):
    frame = create_scale_frame(root, label, var)
    frame.pack(pady=5)

# Result label
result_label = tk.Label(root, text="Output from C program will appear here")
result_label.pack(pady=20)

root.mainloop()
