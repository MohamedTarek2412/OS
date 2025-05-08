import tkinter as tk
from tkinter import ttk, messagebox
import re
from collections import defaultdict

class PrioritySchedulingApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Priority Scheduling (Non-Preemptive) Simulator")
        self.processes = []
        self.create_widgets()

    def create_widgets(self):
        """Create GUI widgets."""
 
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))

        # Number of processes
        ttk.Label(main_frame, text="Number of Processes:").grid(row=0, column=0, sticky=tk.W, pady=5)
        self.num_processes_var = tk.StringVar()
        self.num_processes_entry = ttk.Entry(main_frame, textvariable=self.num_processes_var, width=10)
        self.num_processes_entry.grid(row=0, column=1, sticky=tk.W, pady=5)
        ttk.Button(main_frame, text="Set Processes", command=self.set_processes).grid(row=0, column=2, padx=5, pady=5)


        self.input_frame = ttk.LabelFrame(main_frame, text="Process Details", padding="5")
        self.input_frame.grid(row=1, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=10)
        self.process_entries = []

        self.results_frame = ttk.LabelFrame(main_frame, text="Results", padding="5")
        self.results_frame.grid(row=2, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=10)
        self.results_tree = ttk.Treeview(self.results_frame, columns=(
            "PID", "Arrival", "Burst", "Priority", "Waiting", "Turnaround", "Response"), show="headings")
        self.results_tree.heading("PID", text="PID")
        self.results_tree.heading("Arrival", text="Arrival Time")
        self.results_tree.heading("Burst", text="Burst Time")
        self.results_tree.heading("Priority", text="Priority")
        self.results_tree.heading("Waiting", text="Waiting Time")
        self.results_tree.heading("Turnaround", text="Turnaround Time")
        self.results_tree.heading("Response", text="Response Time")
        self.results_tree.column("PID", width=50)
        self.results_tree.column("Arrival", width=80)
        self.results_tree.column("Burst", width=80)
        self.results_tree.column("Priority", width=80)
        self.results_tree.column("Waiting", width=80)
        self.results_tree.column("Turnaround", width=100)
        self.results_tree.column("Response", width=100)
        self.results_tree.grid(row=0, column=0, sticky=(tk.W, tk.E))
        scrollbar = ttk.Scrollbar(self.results_frame, orient=tk.VERTICAL, command=self.results_tree.yview)
        scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        self.results_tree.configure(yscrollcommand=scrollbar.set)


        self.avg_frame = ttk.LabelFrame(main_frame, text="Averages", padding="5")
        self.avg_frame.grid(row=3, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=10)
        self.avg_waiting_label = ttk.Label(self.avg_frame, text="Avg Waiting Time: N/A")
        self.avg_waiting_label.grid(row=0, column=0, padx=5)
        self.avg_turnaround_label = ttk.Label(self.avg_frame, text="Avg Turnaround Time: N/A")
        self.avg_turnaround_label.grid(row=0, column=1, padx=5)
        self.avg_response_label = ttk.Label(self.avg_frame, text="Avg Response Time: N/A")
        self.avg_response_label.grid(row=0, column=2, padx=5)

        # Gantt chart
        self.gantt_frame = ttk.LabelFrame(main_frame, text="Gantt Chart", padding="5")
        self.gantt_frame.grid(row=4, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=10)
        self.gantt_canvas = tk.Canvas(self.gantt_frame, height=100, width=600, bg="white")
        self.gantt_canvas.grid(row=0, column=0, sticky=(tk.W, tk.E))
        self.gantt_scroll = ttk.Scrollbar(self.gantt_frame, orient=tk.HORIZONTAL, command=self.gantt_canvas.xview)
        self.gantt_scroll.grid(row=1, column=0, sticky=(tk.W, tk.E))
        self.gantt_canvas.configure(xscrollcommand=self.gantt_scroll.set)


        ttk.Button(main_frame, text="Run Simulation", command=self.run_simulation).grid(row=5, column=0, columnspan=3, pady=10)

    def validate_number(self, value):
        """Validate that input is a non-negative number."""
        if value == "":
            return True
        if re.match(r"^\d*\.?\d*$", value) and float(value) >= 0:
            return True
        return False

    def set_processes(self):
        """Set up input fields for processes."""
        try:
            num = int(self.num_processes_var.get())
            if num <= 0:
                raise ValueError("Number of processes must be positive.")
        except ValueError:
            messagebox.showerror("Error", "Please enter a valid positive integer for number of processes.")
            return


        for widget in self.input_frame.winfo_children():
            widget.destroy()
        self.process_entries = []
        self.processes = []

 
        ttk.Label(self.input_frame, text="PID").grid(row=0, column=0, padx=5, pady=5)
        ttk.Label(self.input_frame, text="Arrival Time").grid(row=0, column=1, padx=5, pady=5)
        ttk.Label(self.input_frame, text="Burst Time").grid(row=0, column=2, padx=5, pady=5)
        ttk.Label(self.input_frame, text="Priority").grid(row=0, column=3, padx=5, pady=5)

        for i in range(num):
            pid = f"P{i+1}"
            ttk.Label(self.input_frame, text=pid).grid(row=i+1, column=0, padx=5, pady=5)
            arrival_var = tk.StringVar()
            burst_var = tk.StringVar()
            priority_var = tk.StringVar()
            arrival_entry = ttk.Entry(self.input_frame, textvariable=arrival_var, width=10)
            burst_entry = ttk.Entry(self.input_frame, textvariable=burst_var, width=10)
            priority_entry = ttk.Entry(self.input_frame, textvariable=priority_var, width=10)
            arrival_entry.grid(row=i+1, column=1, padx=5, pady=5)
            burst_entry.grid(row=i+1, column=2, padx=5, pady=5)
            priority_entry.grid(row=i+1, column=3, padx=5, pady=5)
            self.process_entries.append((pid, arrival_var, burst_var, priority_var))

    def run_simulation(self):
        """Run the priority scheduling simulation."""
        self.processes = []
        for pid, arrival_var, burst_var, priority_var in self.process_entries:
            try:
                arrival = float(arrival_var.get())
                burst = float(burst_var.get())
                priority = float(priority_var.get())
                if arrival < 0 or burst <= 0 or priority < 0:
                    raise ValueError("Inputs must be non-negative, and burst time must be positive.")
            except ValueError:
                messagebox.showerror("Error", f"Invalid input for process {pid}. Ensure all fields contain valid non-negative numbers.")
                return
            self.processes.append({
                "pid": pid,
                "arrival": arrival,
                "burst": burst,
                "priority": priority,
                "remaining": burst
            })

        if not self.processes:
            messagebox.showerror("Error", "No processes to simulate.")
            return

        current_time = 0
        completed = []
        gantt = []
        ready_queue = []
        first_run = defaultdict(lambda: None)

        while len(completed) < len(self.processes):
      
            for p in self.processes:
                if p["arrival"] <= current_time and p not in completed and p not in ready_queue:
                    ready_queue.append(p)

            if ready_queue:
                # Select process with highest priority (lowest priority number)
                ready_queue.sort(key=lambda x: x["priority"])
                current_process = ready_queue.pop(0)
                if first_run[current_process["pid"]] is None:
                    first_run[current_process["pid"]] = current_time
                gantt.append((current_process["pid"], current_time, current_time + current_process["remaining"]))
                current_time += current_process["remaining"]
                current_process["completion"] = current_time
                completed.append(current_process)
            else:
           
                current_time += 1

        # Calculate times
        for p in completed:
            p["response"] = first_run[p["pid"]] - p["arrival"]
            p["turnaround"] = p["completion"] - p["arrival"]
            p["waiting"] = p["turnaround"] - p["burst"]


        self.results_tree.delete(*self.results_tree.get_children())
        for p in completed:
            self.results_tree.insert("", tk.END, values=(
                p["pid"], p["arrival"], p["burst"], p["priority"],
                p["waiting"], p["turnaround"], p["response"]
            ))


        avg_waiting = sum(p["waiting"] for p in completed) / len(completed)
        avg_turnaround = sum(p["turnaround"] for p in completed) / len(completed)
        avg_response = sum(p["response"] for p in completed) / len(completed)
        self.avg_waiting_label.config(text=f"Avg Waiting Time: {avg_waiting:.2f}")
        self.avg_turnaround_label.config(text=f"Avg Turnaround Time: {avg_turnaround:.2f}")
        self.avg_response_label.config(text=f"Avg Response Time: {avg_response:.2f}")

      
        self.gantt_canvas.delete("all")
        max_time = max(entry[2] for entry in gantt)
        scale = 500 / max_time if max_time > 0 else 1
        y_base = 50
        self.gantt_canvas.create_line(0, y_base + 20, 600, y_base + 20, fill="black")
        for i, (pid, start, end) in enumerate(gantt):
            x_start = start * scale
            x_end = end * scale
            self.gantt_canvas.create_rectangle(x_start, y_base, x_end, y_base + 20, fill="lightblue", outline="black")
            self.gantt_canvas.create_text((x_start + x_end) / 2, y_base + 10, text=pid)
            self.gantt_canvas.create_text(x_start, y_base + 30, text=f"{start}", anchor=tk.N)
            if i == len(gantt) - 1:
                self.gantt_canvas.create_text(x_end, y_base + 30, text=f"{end}", anchor=tk.N)
        self.gantt_canvas.configure(scrollregion=(0, 0, max_time * scale, 100))

if __name__ == "__main__":
    root = tk.Tk()
    app = PrioritySchedulingApp(root)
    root.mainloop()
