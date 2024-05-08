import tkinter as tk
import serial.tools.list_ports
import serial

def send_command():
    port = port_var.get()
    input_value = entry.get()
    
    if not port:
        result_label.config(text="Please select a port.")
        return
    
    ser = None  # Initialize `ser` to None
    try:
        ser = serial.Serial(port, 9600, timeout=1)
        ser.write(input_value.encode())
        print("encoded_value---------------->",input_value.encode())
        result_label.config(text=f"Command sent to {port}: {input_value}")
    except serial.SerialException as e:
        result_label.config(text=f"Error: Could not open {port}. {str(e)}")
    finally:
        if ser and ser.is_open:  # Check if `ser` is not None and is open
            ser.close()

# Create the main window
root = tk.Tk()
root.title("ESP32 LED Control")

# Find available serial ports
ports = [port.device for port in serial.tools.list_ports.comports()]

# Port selection dropdown
port_var = tk.StringVar(root)
port_var.set(ports[0] if ports else "")
port_dropdown = tk.OptionMenu(root, port_var, *ports)
port_dropdown.pack()

# User input entry
entry_label = tk.Label(root, text="Enter '1' to turn on the LED:")
entry_label.pack()
entry = tk.Entry(root)
entry.pack()

# Send command button
send_button = tk.Button(root, text="Send Command", command=send_command)
send_button.pack()

# Result label
result_label = tk.Label(root, text="")
result_label.pack()

# Run the Tkinter event loop
root.mainloop()
