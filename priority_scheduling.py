import tkinter as tk
from tkinter import ttk, messagebox

class PrioritySchedulingApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Priority Scheduling Simulator")
        self.processes = []
        self.entries = []
        self.create_widgets()

    def create_widgets(self):
        frame = ttk.Frame(self.root, padding="10")
        frame.grid(row=0, column=0, sticky="nsew")

        ttk.Label(frame, text="Number of Processes:").grid(row=0, column=0, sticky="w")
        self.num_proc_var = tk.StringVar()
        ttk.Entry(frame, textvariable=self.num_proc_var, width=5).grid(row=0, column=1)
        ttk.Button(frame, text="Setup", command=self.setup_processes).grid(row=0, column=2, padx=5)

        self.input_frame = ttk.Frame(frame)
        self.input_frame.grid(row=1, column=0, columnspan=3, pady=10)

        self.tree = ttk.Treeview(frame, columns=("PID", "Arrival", "Burst", "Priority", "Waiting", "Turnaround", "Response"), show="headings")
        for col in self.tree["columns"]:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=80)
        self.tree.grid(row=2, column=0, columnspan=3, sticky="nsew")

        self.avg_label = ttk.Label(frame, text="Averages: Waiting = N/A, Turnaround = N/A, Response = N/A")
        self.avg_label.grid(row=3, column=0, columnspan=3, pady=5)

        self.gantt_canvas = tk.Canvas(frame, height=60, width=400, bg="white")
        self.gantt_canvas.grid(row=4, column=0, columnspan=3, pady=10)

        ttk.Button(frame, text="Run", command=self.run_simulation).grid(row=5, column=0, columnspan=3, pady=10)

    def setup_processes(self):
        try:
            num = int(self.num_proc_var.get())
            if num < 1:
                raise ValueError
        except ValueError:
            messagebox.showerror("Error", "Enter a valid positive number of processes")
            return

        for widget in self.input_frame.winfo_children():
            widget.destroy()
        self.entries = []

        headers = ["PID", "Arrival", "Burst", "Priority"]
        for j, header in enumerate(headers):
            ttk.Label(self.input_frame, text=header).grid(row=0, column=j, padx=5)
        for i in range(num):
            pid = f"P{i+1}"
            ttk.Label(self.input_frame, text=pid).grid(row=i+1, column=0, padx=5)
            arrival = tk.StringVar()
            burst = tk.StringVar()
            priority = tk.StringVar()
            ttk.Entry(self.input_frame, textvariable=arrival, width=5).grid(row=i+1, column=1)
            ttk.Entry(self.input_frame, textvariable=burst, width=5).grid(row=i+1, column=2)
            ttk.Entry(self.input_frame, textvariable=priority, width=5).grid(row=i+1, column=3)
            self.entries.append((pid, arrival, burst, priority))

    def run_simulation(self):
        self.processes = []
        for pid, arrival_var, burst_var, priority_var in self.entries:
            try:
                arrival = float(arrival_var.get())
                burst = float(burst_var.get())
                priority = float(priority_var.get())
                if arrival < 0 or burst <= 0 or priority < 0:
                    raise ValueError
            except ValueError:
                messagebox.showerror("Error", f"Invalid input for {pid}. Use non-negative numbers (burst > 0).")
                return
            self.processes.append({"pid": pid, "arrival": arrival, "burst": burst, "priority": priority})

        if not self.processes:
            messagebox.showerror("Error", "No processes to simulate")
            return

        current_time = 0
        completed = []
        gantt = []
        first_run = {}

        while len(completed) < len(self.processes):
            ready = [p for p in self.processes if p["arrival"] <= current_time and p not in completed]
            if ready:
                current_process = min(ready, key=lambda x: x["priority"])
                if current_process["pid"] not in first_run:
                    first_run[current_process["pid"]] = current_time
                start_time = current_time
                current_time += current_process["burst"]
                gantt.append((current_process["pid"], start_time, current_time))
                current_process["completion"] = current_time
                current_process["turnaround"] = current_time - current_process["arrival"]
                current_process["waiting"] = current_process["turnaround"] - current_process["burst"]
                current_process["response"] = first_run[current_process["pid"]] - current_process["arrival"]
                completed.append(current_process)
            else:
                # 🔸 حالة IDLE لما مفيش عملية جاهزة
                idle_start = current_time
                current_time += 1
                gantt.append(("IDLE", idle_start, current_time))

        self.tree.delete(*self.tree.get_children())
        for p in sorted(completed, key=lambda x: x["pid"]):
            self.tree.insert("", "end", values=(
                p["pid"], p["arrival"], p["burst"], p["priority"],
                f"{p['waiting']:.1f}", f"{p['turnaround']:.1f}", f"{p['response']:.1f}"
            ))

        avg_waiting = sum(p["waiting"] for p in completed) / len(completed)
        avg_turnaround = sum(p["turnaround"] for p in completed) / len(completed)
        avg_response = sum(p["response"] for p in completed) / len(completed)
        self.avg_label.config(text=f"Averages: Waiting = {avg_waiting:.1f}, Turnaround = {avg_turnaround:.1f}, Response = {avg_response:.1f}")

        self.gantt_canvas.delete("all")
        max_time = max(end for _, _, end in gantt) if gantt else 1
        scale = 350 / max_time
        y = 30
        for pid, start, end in gantt:
            x1 = start * scale
            x2 = end * scale
            color = "lightblue" if pid != "IDLE" else "lightgray"
            self.gantt_canvas.create_rectangle(x1, y-10, x2, y+10, fill=color)
            self.gantt_canvas.create_text((x1+x2)/2, y, text=pid)
            self.gantt_canvas.create_text(x1, y+20, text=f"{start:.1f}")
            self.gantt_canvas.create_text(x2, y+20, text=f"{end:.1f}")

if __name__ == "__main__":
    root = tk.Tk()
    app = PrioritySchedulingApp(root)
    root.mainloop()
