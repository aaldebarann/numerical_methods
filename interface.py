import tkinter as tk
from tkinter import ttk
import subprocess
import csv
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import mplcursors

class IntegrationApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Интегратор")
        
        self.top_left_panel = ttk.LabelFrame(root, text="Выбор параметров")
        self.top_left_panel.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")
        
        self.top_right_panel = ttk.LabelFrame(root, text="Сводка")
        self.top_right_panel.grid(row=0, column=1, padx=10, pady=10, sticky="nsew")
        
        self.bottom_left_panel = ttk.LabelFrame(root, text="График")
        self.bottom_left_panel.grid(row=1, column=0, padx=10, pady=10, sticky="nsew")
        
        self.bottom_right_panel = ttk.LabelFrame(root, text="Результаты")
        self.bottom_right_panel.grid(row=1, column=1, padx=10, pady=10, sticky="nsew")
   
        self.create_top_left_panel()
        self.create_top_right_panel()
        self.create_bottom_left_panel()
        self.create_bottom_right_panel()
    
    def create_top_left_panel(self):
        self.task_label = ttk.Label(self.top_left_panel, text="Выбор задачи:")
        self.task_label.grid(row=0, column=0, sticky="w")
        self.task_var = tk.StringVar()
        self.task_combo = ttk.Combobox(self.top_left_panel, textvariable=self.task_var, values=["Тестовая", "Первая", "Вторая"])
        self.task_combo.grid(row=0, column=1)
        self.task_var.set("Тестовая")
        
        self.method_label = ttk.Label(self.top_left_panel, text="Выбор типа метода:")
        self.method_label.grid(row=1, column=0, sticky="w")
        self.method_var = tk.StringVar()
        self.method_combo = ttk.Combobox(self.top_left_panel, textvariable=self.method_var, values=["С постоянным шагом", "С переменным шагом"])
        self.method_combo.grid(row=1, column=1)
        self.method_var.set("С постоянным шагом")
        
        self.start_label = ttk.Label(self.top_left_panel, text="Начало интервала:")
        self.start_label.grid(row=2, column=0, sticky="w")
        self.start_var = tk.DoubleVar()
        self.start_entry = ttk.Entry(self.top_left_panel, textvariable=self.start_var)
        self.start_entry.grid(row=2, column=1)
        self.start_var.set("0.0")
        
        self.end_label = ttk.Label(self.top_left_panel, text="Конец интервала:")
        self.end_label.grid(row=3, column=0, sticky="w")
        self.end_var = tk.DoubleVar()
        self.end_entry = ttk.Entry(self.top_left_panel, textvariable=self.end_var)
        self.end_entry.grid(row=3, column=1)
        self.end_var.set("10.0")
        
        self.step_label = ttk.Label(self.top_left_panel, text="Шаг интегрирования:")
        self.step_label.grid(row=4, column=0, sticky="w")
        self.step_var = tk.DoubleVar()
        self.step_entry = ttk.Entry(self.top_left_panel, textvariable=self.step_var)
        self.step_entry.grid(row=4, column=1)
        self.step_var.set("0.001")
        
        self.precision_label = ttk.Label(self.top_left_panel, text="Точность интегрирования:")
        self.precision_label.grid(row=5, column=0, sticky="w")
        self.precision_var = tk.DoubleVar()
        self.precision_entry = ttk.Entry(self.top_left_panel, textvariable=self.precision_var)
        self.precision_entry.grid(row=5, column=1)
        self.precision_var.set("0.0001")
        
        self.max_steps_label = ttk.Label(self.top_left_panel, text="Максимальное количество шагов:")
        self.max_steps_label.grid(row=6, column=0, sticky="w")
        self.max_steps_var = tk.IntVar()
        self.max_steps_entry = ttk.Entry(self.top_left_panel, textvariable=self.max_steps_var)
        self.max_steps_entry.grid(row=6, column=1)
        self.max_steps_var.set("1000000")
        
        self.initial_condition_label = ttk.Label(self.top_left_panel, text="Начальное условие для u:")
        self.initial_condition_label.grid(row=7, column=0, sticky="w")
        self.initial_condition_var = tk.DoubleVar()
        self.initial_condition_entry = ttk.Entry(self.top_left_panel, textvariable=self.initial_condition_var)
        self.initial_condition_entry.grid(row=7, column=1)
        self.initial_condition_var.set("1")
        
        self.second_initial_condition_label = ttk.Label(self.top_left_panel, text="Второе начальное условие для u':")
        self.second_initial_condition_label.grid(row=8, column=0, sticky="w")
        self.second_initial_condition_var = tk.DoubleVar()
        self.second_initial_condition_entry = ttk.Entry(self.top_left_panel, textvariable=self.second_initial_condition_var)
        self.second_initial_condition_entry.grid(row=8, column=1)
        self.second_initial_condition_var.set("0")
        
        self.parameter_label = ttk.Label(self.top_left_panel, text="Значение параметра:")
        self.parameter_label.grid(row=9, column=0, sticky="w")
        self.parameter_var = tk.DoubleVar()
        self.parameter_entry = ttk.Entry(self.top_left_panel, textvariable=self.parameter_var)
        self.parameter_entry.grid(row=9, column=1)
        self.parameter_var.set("1")
        
        self.run_button = ttk.Button(self.top_left_panel, text="Выполнить", command=self.run_integration)
        self.run_button.grid(row=10, column=0)
        
        self.clear_button = ttk.Button(self.top_left_panel, text="Очистить график", command=self.clear_plot)
        self.clear_button.grid(row=10, column=1)
    
    def create_top_right_panel(self):
        self.summary_label = ttk.Label(self.top_right_panel, text="Сводка:")
        self.summary_label.grid(row=0, column=0)

    def create_bottom_left_panel(self):
        self.fig, self.ax = plt.subplots(figsize=(6, 4))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.bottom_left_panel)
        self.canvas.get_tk_widget().pack()
    
    def create_bottom_right_panel(self):
        self.results_label = ttk.Label(self.bottom_right_panel, text="Результаты:")
        self.results_label.grid(row=0, column=0)
        self.notebook = ttk.Notebook(self.bottom_right_panel)
        self.notebook.grid(row=0, column=0, sticky="nsew")
        
    def run_integration(self):
        task = self.task_var.get()
        method = self.method_var.get()
        start = self.start_var.get()
        end = self.end_var.get()
        step = self.step_var.get()
        precision = self.precision_var.get()
        max_steps = self.max_steps_var.get()
        initial_condition = self.initial_condition_var.get()
        second_initial_condition = self.second_initial_condition_var.get()
        parameter = self.parameter_var.get()
        m = 0
        t = 1
        if method == "С постоянным шагом":
            m = 1
        if task == "Тестовая":
            columns = ["i", "x_i", "v_i", "v2_i", "v_i - v2_i", "ОЛП", "h_i", "C1", "C2", "u_i", "|u_i - v_i|"]
        if task == "Первая":
            t = 2
            columns = ["i", "x_i", "v_i", "v2_i", "v_i - v2_i", "ОЛП", "h_i", "C1", "C2"]
        elif task == "Вторая":
            t = 3
            columns = ["i", "x_i", "dv_i", "v_i", "dv2_i", "v2_i", "dv_i - dv2_i", "v_i - v2_i", "ОЛП", "h_i", "C1", "C2"]
        
        style = (t, m)
        with open("input_params.txt", "w", encoding="utf-8") as params_file:
            params_file.write(f"{start} {end} {initial_condition} {precision} {step} {max_steps} {t} {m} {second_initial_condition} {parameter}")
        
        subprocess.run(["./labwork1.exe"], check=True)
        with open('results.csv', 'r') as file:
            reader = csv.reader(file)
            result_tree = self.create_result_tab(columns)
                
            for row in reader:
                values = [float(value) for value in row[0:]]
                result_tree.insert("", "end", values=values)
                
            self.create_plots()
            self.create_summary_table(style)
    
    def create_plots(self):
        x = []
        y1 = []
        y2 = []
        task = self.task_var.get()
        
        with open('results.csv', 'r') as file:
            reader = csv.reader(file)
            for row in reader:
                x.append(float(row[1]))
                y1.append(float(row[2]))
                if task == "Тестовая":
                    y2.append(float(row[9]))
                if task == "Вторая":
                    y2.append(float(row[3]))
        
        if task == "Тестовая":
            self.ax.plot(x, y1, 'o', markersize = 1, label="Тестовая численная")
            self.ax.plot(x, y2, 'o', markersize = 1, label="Тестовая истинная")
        elif task == "Первая":
            self.ax.plot(x, y1, 'o', markersize = 1, label="Первая")
        elif task == "Вторая":
            self.ax.plot(x, y1, 'o', markersize = 1, label="Вторая (производная)")
            self.ax.plot(x, y2, 'o', markersize = 1, label="Вторая")
            self.open_new_graph(y1, y2)
            
        
        self.ax.set_xlabel('x')
        self.ax.set_ylabel('u')
        self.ax.set_title('График функции')
        self.ax.grid(True)
        self.ax.legend(loc='upper left', title="Легенда")
        self.canvas.draw()
    
        cursor = mplcursors.cursor(self.ax, hover=True)
        cursor.connect("add", lambda sel: sel.annotation.set_text(f"x={sel.target[0]:.2f}, y={sel.target[1]:.2f}"))
        
    def clear_plot(self):
        self.ax.clear()
        self.ax.set_xlabel('x')
        self.ax.set_ylabel('u')
        self.ax.set_title('График функции')
        self.ax.grid(True)
        self.canvas.draw()
        for i in range(self.notebook.index("end") - 1, -1, -1):
            self.notebook.forget(i)
                
        
    def create_result_tab(self, columns):
        result_tab = ttk.Frame(self.notebook)
        counter = self.notebook.index("end") + 1
        self.notebook.add(result_tab, text=f"{counter}")
        result_tree = ttk.Treeview(result_tab, columns=columns, show="headings")
        for column in columns:
            result_tree.heading(column, text=column)
            result_tree.column(column, width=80)
        result_tree.pack(fill=tk.BOTH, expand=True)

        return result_tree

    def create_summary_table(self, style):
        summary_tab = ttk.Frame(self.top_right_panel)
        summary_tab.grid(row=0, column=0, sticky="nsew")
        summary_tree = ttk.Treeview(summary_tab, show="headings")
        summary_tree.pack(fill=tk.BOTH, expand=True)
        if style == (1, 1):
            headers = ["n", "b - x_n", "max|ОЛП|", "max|u_i - v_i|", "x"]
        elif style == (1, 0):
            headers = ["n", "b - x_n", "max|ОЛП|", "C1", "C2", "max h_i", "х", "min h_i", "х", "max|u_i - v_i|", "x"]
        elif style == (2, 0) or style == (3, 0):
            headers = ["n", "b - x_n", "max|ОЛП|", "C1", "C2", "max h_i", "х", "min h_i", "х"]   
        elif style == (2, 1) or style == (3, 1):
            headers = ["n", "b - x_n", "max|ОЛП|"]
            
        try:
            with open('summary.csv', 'r') as file:
                reader = csv.reader(file)
                summary_tree["columns"] = headers
                for header in headers:
                    summary_tree.heading(header, text=header)
                    summary_tree.column(header, width = 80)
                for row in reader:
                    summary_tree.insert("", "end", values=row)
    
        except FileNotFoundError:
            print("Файл 'summary.csv' не найден")
    
        return summary_tree
    
    def open_new_graph(self, u, v):
        new_window = tk.Toplevel(root)
        new_window.title("Фазовый портрет")

        new_fig, new_ax = plt.subplots(figsize=(6, 4))
        new_canvas = FigureCanvasTkAgg(new_fig, master=new_window)
        new_canvas.get_tk_widget().pack()

        new_ax.plot(u, v, 'o', markersize = 1,)

        new_ax.set_xlabel('u')
        new_ax.set_ylabel('u\'')
        new_ax.set_title('Фазовый портрет')
        new_ax.grid(True)
        new_canvas.draw()


if __name__ == "__main__":
    root = tk.Tk()
    app = IntegrationApp(root)
    root.mainloop()
