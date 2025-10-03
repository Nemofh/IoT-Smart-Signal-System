from tkcalendar import DateEntry
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import sqlite3
import serial
from fpdf import FPDF
import datetime
import threading
from PIL import ImageTk, Image
import os
import platform
import subprocess


def init_db():
    conn = sqlite3.connect("signals.db")
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS traffic_signals (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            district TEXT NOT NULL,
            streets INTEGER NOT NULL
        )
    ''')
    conn.commit()
    conn.close()


def init_log_table():
    conn = sqlite3.connect("signals.db")
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS timing_logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            signal_id INTEGER,
            district TEXT,
            lane INTEGER NOT NULL,
            old_time INTEGER,
            new_time INTEGER NOT NULL,
            reason TEXT NOT NULL,
            date TEXT NOT NULL,
            time TEXT NOT NULL
        )
    ''')
    conn.commit()
    conn.close()


def log_timing_change(lane, old_time, new_time, reason, signal_id=None, district=None):
    conn = sqlite3.connect("signals.db")
    c = conn.cursor()
    now = datetime.datetime.now()
    date = now.strftime('%Y-%m-%d')
    time = now.strftime('%H:%M:%S')

    c.execute('''
        INSERT INTO timing_logs (signal_id, district, lane, old_time, new_time, reason, date, time)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    ''', (signal_id, district, lane, old_time, new_time, reason, date, time))

    conn.commit()
    conn.close()


def view_timing_logs():
    log_win = tk.Toplevel(root)
    log_win.title("Timing Change Logs")
    log_win.geometry("900x400")
    log_win.configure(bg="#f7fafd")

    tk.Label(log_win, text="Timing Adjustment History", font=(
        "Arial", 14, "bold"), bg="#f7fafd").pack(pady=10)

    columns = ("ID", "Signal ID", "District", "Lane",
               "Old Time", "New Time", "Reason", "Date", "Time")

    container = tk.Frame(log_win)
    container.pack(fill="both", expand=True)

    canvas = tk.Canvas(container)
    canvas.pack(side="left", fill="both", expand=True)

    v_scroll = tk.Scrollbar(container, orient="vertical", command=canvas.yview)
    v_scroll.pack(side="right", fill="y")

    h_scroll = tk.Scrollbar(log_win, orient="horizontal", command=canvas.xview)
    h_scroll.pack(fill="x")

    canvas.configure(yscrollcommand=v_scroll.set, xscrollcommand=h_scroll.set)
    canvas.bind('<Configure>', lambda e: canvas.configure(
        scrollregion=canvas.bbox("all")))

    frame = tk.Frame(canvas)
    canvas.create_window((0, 0), window=frame, anchor="nw")

    tree = ttk.Treeview(frame, columns=columns, show="headings")

    for col in columns:
        tree.heading(col, text=col)
        tree.column(col, width=120, anchor="center")

    tree.pack(fill="both", expand=True)

   
    conn = sqlite3.connect("signals.db")  # Fetch and insert logs
    c = conn.cursor()
    c.execute("SELECT * FROM timing_logs ORDER BY date DESC, time DESC")
    logs = c.fetchall()
    conn.close()

    for log in logs:
        tree.insert("", "end", values=log)


def add_signal_to_db(district, streets):
    conn = sqlite3.connect("signals.db")
    c = conn.cursor()
    c.execute("INSERT INTO traffic_signals (district, streets) VALUES (?, ?)",
              (district, int(streets)))
    conn.commit()
    conn.close()


def view_signals():
    view_win = tk.Toplevel(root)
    view_win.title("Traffic Signal Dashboard")
    view_win.iconbitmap("icon_sign.jpg")
    view_win.geometry("600x500")
    view_win.configure(bg="#dff2f7")

    selected_signal = tk.IntVar(value=0)

    top_bar = tk.Frame(view_win, bg="#dff2f7")
    top_bar.pack(fill="x", pady=5, padx=10)

    search_var = tk.StringVar()
    search_entry = tk.Entry(
        top_bar, textvariable=search_var, font=("Arial", 12))
    search_entry.pack(side="left", expand=True, fill="x", padx=(0, 5))

    def filter_cards():
        term = search_var.get().lower()
        for frame, info in zip(card_frames, signal_data):
            visible = term in info[1].lower() or term in str(info[0])
            frame.pack_forget() if not visible else frame.pack(pady=5, padx=10, fill="x")

    tk.Button(top_bar, text="Search", command=filter_cards).pack(side="left")

    def open_timing_for_selected():
        sid = selected_signal.get()
        if sid == 0:
            messagebox.showwarning(
                "No Selection", "Please select a signal first.")
            return
        open_timing_adjustment(sid)

    timer_btn = tk.Button(top_bar, text="⏱ Adjust Timing", font=(
        "Arial", 12), command=open_timing_for_selected)
    timer_btn.pack(side="right", padx=(5, 0))

    canvas = tk.Canvas(view_win, bg="#dff2f7")
    scrollbar = tk.Scrollbar(view_win, orient="vertical", command=canvas.yview)
    scroll_frame = tk.Frame(canvas, bg="#dff2f7")
    canvas.create_window((0, 0), window=scroll_frame, anchor="nw")
    canvas.configure(yscrollcommand=scrollbar.set)
    canvas.pack(side="left", fill="both", expand=True)
    scrollbar.pack(side="right", fill="y")

    scroll_frame.bind("<Configure>", lambda e: canvas.configure(
        scrollregion=canvas.bbox("all")))

    conn = sqlite3.connect("signals.db")
    c = conn.cursor()
    c.execute("SELECT * FROM traffic_signals")
    signal_data = c.fetchall()
    conn.close()

    card_frames = []
    status_labels = []
    light_canvases = []

    for signal_id, district, streets in signal_data:
        frame = tk.Frame(scroll_frame, bg="white", bd=2, relief="ridge")
        frame.pack(pady=5, padx=10, fill="x")

        status_label = tk.Label(frame, text="Checking...", font=(
            "Arial", 12, "bold"), bg="white")
        status_label.pack(anchor="w", padx=10, pady=(5, 2))

        info_label = tk.Label(frame, text=f"Signal No. {signal_id}, {district}", font=(
            "Arial", 11), bg="white")
        info_label.pack(anchor="w", padx=10)

        light_canvas = tk.Canvas(
            frame, width=80, height=40, bg="white", highlightthickness=0)
        light_canvas.pack(anchor="e", padx=10, pady=5)

        radio = tk.Radiobutton(
            frame, text="Select", variable=selected_signal, value=signal_id, bg="white")
        radio.pack(anchor="e", padx=10, pady=5)

        card_frames.append(frame)
        status_labels.append(status_label)
        light_canvases.append(light_canvas)

    def draw_traffic_lights(canvas, is_green):
        canvas.delete("all")
        canvas.create_rectangle(
            10, 10, 30, 30, fill="green" if is_green else "gray", outline="")
        canvas.create_rectangle(
            50, 10, 70, 30, fill="red" if not is_green else "gray", outline="")

    def serial_listener():
        try:
            ser = serial.Serial("COM3", 9600)
        except Exception as e:
            print("Serial Error:", e)
            return

        while True:
            try:
                line = ser.readline().decode().strip()
                if line.startswith("Status-"):
                    _, l1, l2, _, _ = line.split("-")
                    lane1 = l1 == "2"
                    lane2 = l2 == "2"

                    if len(status_labels) > 0:
                        status_labels[0]["text"] = "Heavy Traffic" if lane1 else "No Traffic"
                        draw_traffic_lights(light_canvases[0], lane1)
                    if len(status_labels) > 1:
                        status_labels[1]["text"] = "Heavy Traffic" if lane2 else "No Traffic"
                        draw_traffic_lights(light_canvases[1], lane2)
                elif line.startswith("AutoChange-"):
                    try:
                        _, lane, time_value = line.split("-")
                        lane = int(lane)
                        time_value = int(time_value)
                        district = "Main Junction"
                        reason = ""
                        if lane == 1:
                            reason = "Heavy traffic in Sign1 Lane 1 while Lane 2 was empty"
                            signal_id = 1 
                        elif lane == 2:
                            reason = "Heavy traffic in Sign2 Lane 2 while Lane 1 was empty"
                            signal_id = 2 
                        else:
                            reason = "Auto Adjustment"

                        log_timing_change(lane, time_value,
                                          reason, signal_id, district)
                    except Exception as err:
                        print("AutoChange parse error:", err)

            except:
                break
    
    threading.Thread(target=serial_listener, daemon=True).start()


def add_signal_window():
    add_win = tk.Toplevel(root)
    add_win.title("Add Signal")
    add_win.geometry("400x300")
    add_win.configure(bg="#d8e7f0")

    back_button = tk.Button(add_win, text="⟵", font=(
        "Arial", 12), command=add_win.destroy, bg="#d8e7f0", bd=0)
    back_button.place(x=10, y=10)

    title_frame = tk.Frame(add_win, bg="#d8e7f0", bd=2, relief="solid")
    title_frame.pack(pady=20)
    title_label = tk.Label(title_frame, text="Add Signal",
                           font=("Arial", 16, "bold"), bg="#d8e7f0")
    title_label.pack(padx=10, pady=5)

    district_label = tk.Label(add_win, text="District Name", font=(
        "Arial", 12, "bold"), bg="#d8e7f0")
    district_label.pack(pady=(10, 0))
    district_entry = tk.Entry(add_win, font=("Arial", 12))
    district_entry.pack(pady=5)

    streets_label = tk.Label(add_win, text="Number of Streets", font=(
        "Arial", 12, "bold"), bg="#d8e7f0")
    streets_label.pack(pady=(10, 0))
    streets_options = ["2", "3", "4", "5", "6"]
    streets_dropdown = ttk.Combobox(
        add_win, values=streets_options, font=("Arial", 12), state="readonly")
    streets_dropdown.pack(pady=5)

    def submit_signal():
        district = district_entry.get()
        streets = streets_dropdown.get()
        if district and streets:
            add_signal_to_db(district, streets)
            messagebox.showinfo(
                "Success", f"Signal added for {district} with {streets} streets.")
            add_win.destroy()
        else:
            messagebox.showwarning("Input Error", "Please fill all fields.")

    add_button = tk.Button(add_win, text="Add", font=(
        "Arial", 12, "bold"), command=submit_signal, relief="raised")
    add_button.pack(pady=20)

    def submit_signal():
        district = district_entry.get()
        streets = streets_dropdown.get()
        if district and streets:
            messagebox.showinfo(
                "Success", f"Signal added for {district} with {streets} streets.")
            add_win.destroy()
        else:
            messagebox.showwarning("Input Error", "Please fill all fields.")

    add_button = tk.Button(add_win, text="Add", font=(
        "Arial", 12, "bold"), command=submit_signal, relief="raised")
    add_button.pack(pady=20)


def generate_reports():
    report_win = tk.Toplevel()
    report_win.title("Generate Report")
    report_win.geometry("500x400")
    report_win.configure(bg="#f0f8ff")

    tk.Label(report_win, text="Generate PDF Report", font=("Arial", 14, "bold"), bg="#f0f8ff").pack(pady=10)

    date_frame = tk.Frame(report_win, bg="#f0f8ff")
    date_frame.pack(pady=10)

    tk.Label(date_frame, text="Start Date:", font=("Arial", 11), bg="#f0f8ff").grid(row=0, column=0, padx=5)
    start_date = DateEntry(date_frame, width=12, background='darkblue', foreground='white', date_pattern='yyyy-mm-dd')
    start_date.grid(row=0, column=1, padx=5)

    tk.Label(date_frame, text="End Date:", font=("Arial", 11), bg="#f0f8ff").grid(row=0, column=2, padx=5)
    end_date = DateEntry(date_frame, width=12, background='darkblue', foreground='white', date_pattern='yyyy-mm-dd')
    end_date.grid(row=0, column=3, padx=5)

    def export_report():
        conn = sqlite3.connect("signals.db")
        c = conn.cursor()
        c.execute('''
            SELECT signal_id, district, lane, old_time, new_time, reason, date, time
            FROM timing_logs
            WHERE date BETWEEN ? AND ?
            ORDER BY date ASC, time ASC
        ''', (start_date.get_date().strftime('%Y-%m-%d'), end_date.get_date().strftime('%Y-%m-%d')))
        logs = c.fetchall()
        conn.close()

        if not logs:
            messagebox.showinfo("No Data", "No timing changes found in the selected date range.")
            return

        pdf = FPDF()
        pdf.add_page()
        pdf.set_font("Arial", "B", 14)
        pdf.cell(0, 10, "Traffic Timing Adjustment Report", ln=True, align="C")

        pdf.set_font("Arial", "", 11)
        pdf.cell(0, 10, f"From {start_date.get()} to {end_date.get()}", ln=True)
        pdf.ln(5)

        headers = ["Signal ID", "District", "Lane", "Old", "New", "Reason", "Date", "Time"]
        column_widths = [20, 30, 15, 15, 15, 60, 25, 20]

        pdf.set_font("Arial", "B", 10)
        for i, header in enumerate(headers):
            pdf.cell(column_widths[i], 8, header, 1)
        pdf.ln()

        pdf.set_font("Arial", "", 9)
        for row in logs:
            for i, item in enumerate(row):
                pdf.cell(column_widths[i], 8, str(item), 1)
            pdf.ln()

        file_path = f"report_{start_date.get()}_to_{end_date.get()}.pdf"
        pdf.output(file_path)
        tk.Label(report_win, text=f"Report saved as: {file_path}", font=("Arial", 10), bg="#f0f8ff").pack(pady=10)

        def open_file_location():
            abs_path = os.path.abspath(file_path)
            folder_path = os.path.dirname(abs_path)
            if platform.system() == "Windows":
                os.startfile(folder_path)
            elif platform.system() == "Darwin":
                subprocess.run(["open", folder_path])
            else:
                subprocess.run(["xdg-open", folder_path])

        tk.Button(report_win, text="Open Folder", command=open_file_location, font=("Arial", 10)).pack(pady=5)

    tk.Button(report_win, text="Export PDF", command=export_report, font=("Arial", 12, "bold")).pack(pady=20)


def close_app():
    root.destroy()


def open_timing_adjustment(signal_id):
    timing_window = tk.Toplevel(root)
    timing_window.title("Adjust Timing")
    timing_window.geometry("400x250")
    timing_window.configure(bg="#eaf4f7")

    tk.Label(timing_window, text=f"Adjust Timing for Signal {signal_id}", font=(
        "Arial", 14, "bold"), bg="#eaf4f7").pack(pady=10)

    # Fetch latest timing values for this signal from logs
    conn = sqlite3.connect("signals.db")
    c = conn.cursor()
    c.execute("SELECT lane, new_time FROM timing_logs WHERE signal_id = ? ORDER BY date DESC, time DESC", (signal_id,))
    fetched = c.fetchall()
    conn.close()

    latest = {}
    for lane, new_time in fetched:
        if lane not in latest:
            latest[lane] = new_time

    times = [tk.IntVar(value=latest.get(1, 30)),
             tk.IntVar(value=latest.get(2, 30))]

    for i in range(2):
        frame = tk.Frame(timing_window, bg="#ffffff", bd=1, relief="solid")
        frame.pack(fill="x", padx=20, pady=10)

        tk.Label(frame, text=f"Lane {i+1}", font=("Arial", 12),
                 bg="white").grid(row=0, column=0, padx=10, pady=10)
        green_label = tk.Label(
            frame, textvariable=times[i], font=("Arial", 12), bg="white")
        green_label.grid(row=0, column=1)

        def inc(idx=i): times[idx].set(times[idx].get() + 5)
        def dec(idx=i): times[idx].set(max(5, times[idx].get() - 5))

        tk.Button(frame, text="-", command=dec, width=2).grid(row=0, column=2)
        tk.Button(frame, text="+", command=inc, width=2).grid(row=0, column=3)

    def send_to_arduino():
        try:
            ser = serial.Serial("COM3", 9600)
            lane1_new = times[0].get()
            lane2_new = times[1].get()
            ser.write(f"1-{lane1_new}\n".encode())
            ser.write(f"2-{lane2_new}\n".encode())
            ser.close()

            # Retrieve district for logging
            conn = sqlite3.connect("signals.db")
            c = conn.cursor()
            c.execute(
                "SELECT district FROM traffic_signals WHERE id = ?", (signal_id,))
            row = c.fetchone()
            district = row[0] if row else "Unknown"

            c.execute(
                "SELECT lane, new_time FROM timing_logs WHERE signal_id = ? ORDER BY date DESC, time DESC", (signal_id,))
            logs = c.fetchall()
            conn.close()

            latest = {}
            for lane, t in logs:
                if lane not in latest:
                    latest[lane] = t

            old1 = latest.get(1, 30)
            old2 = latest.get(2, 30)

            # Log with old and new
            log_timing_change(1, old1, lane1_new,
                              "Manual Adjustment", signal_id, district)
            log_timing_change(2, old2, lane2_new,
                              "Manual Adjustment", signal_id, district)

            tk.messagebox.showinfo(
                "Success", "Timing updated and sent to Arduino!")
            timing_window.destroy()
        except Exception as e:
            tk.messagebox.showerror(
                "Serial Error", f"Could not send to Arduino:\n{e}")

    tk.Button(timing_window, text="Save & Send to Arduino", font=(
        "Arial", 12), command=send_to_arduino).pack(pady=20)


root = tk.Tk()
root.title("Signal Management")
root.iconbitmap("icon_sign.ico")
root.geometry("400x650")
root.configure(bg="#d8e7f0")


img=Image.open('icon_sign.ico')
img=img.resize((150, 200))
logo_icon = ImageTk.PhotoImage(img)

logo = tk.Label(root, image=logo_icon, font=("Arial", 40), bg="#d8e7f0")
logo.pack(pady=20)
init_db()
init_log_table()
button_style = {"font": ("Arial", 14), "width": 20, "height": 2}
btn_view = tk.Button(root, text="View Signals",
                     command=view_signals, **button_style)
btn_view.pack(pady=10)
btn_add = tk.Button(root, text="Add Signal",
                    command=add_signal_window, **button_style)
btn_add.pack(pady=10)
btn_generate = tk.Button(root, text="Generate Reports",
                         command=generate_reports, **button_style)
btn_generate.pack(pady=10)
btn_logs = tk.Button(root, text="View Timing Logs",
                     command=view_timing_logs, **button_style)
btn_logs.pack(pady=10)
btn_close = tk.Button(root, text="Close", command=close_app, **button_style)
btn_close.pack(pady=20)
root.mainloop()
