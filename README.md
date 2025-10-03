<p align="right">
  <img src="https://i.imgur.com/oKa0kuq.png" width="200" />
</p>


# 🚦  Smart Signal Priority System  


Smart Signal Priority System is a Python-based desktop application designed to optimize traffic flow at intersections.
Instead of relying on fixed signal timings, the system uses sensor data collected via Arduino to detect real-time traffic congestion and automatically prioritize the most congested direction by extending the green light duration.

#### This Application is developed by: Laym Alghimlas,Rawan Alanazi,Yara Alshbrami.






![App Screenshot](https://i.imgur.com/UFJdz8B.gif)


## Application Objectives:

- Utilize Real-Time Traffic Sensors.
- Develop a Dynamic Signal Control Algorithm.
- Implement a User Interface for Signal Management.
- Enable Manual and Automated Control Modes.

The contribution of this project is to reduce traffic congestion at intersections by giving priority to directions experiencing higher traffic pressure.
This enhances traffic flow, reduces the likelihood of accidents, minimizes waiting times, and improves the overall driving experience on public roads.
##  Technologies Used

**The team members used:**
- Python programming language.
- a graphical interface built with Tkinter. 
- an SQLite database.
- in addition to integrating with an Arduino board using the PySerial library to build this desktop application.
## ⚙️Installation and Setup Instructions:

 1. Install Python on your computer.
 Make sure you have **Python 3.x** installed on your system.  
You can download it from [python.org](https://www.python.org/downloads/).


2. 🧩 **Recommended Editor:**  
It is strongly recommended to use **Visual Studio Code (VS Code)** for editing and running the project, as it offers an integrated terminal, Python extensions, and smooth file management.

> 💡 **Note:** After opening the project in VS Code, install the **Python Extension** from the Extensions Marketplace if it's not already installed.


- ## 📁 Project Folder Setup:

  - Clone or download the project folder.

  - Open VS Code → go to File > Open Folder... and select the project directory.

 **Make sure all .py files (including main_gui_template.py) and the database file signals.db are in the same directory.**

    - ⚠️Install the required libraries via the terminal using the following commands:
> 💡 **Note :Install the required libraries using pip📦.**
  ```bash
  pip install tkcalendar
  pip install pyserial
  pip install fpdf
  pip install pillow
 ```
3. Download the Arduino IDE and upload the Arduino code to the board.

4. Connect the Arduino board to the computer via USB cable.

5. Run the Python file using the following command:
```bash
python main_gui_template.py
```
## ⚡ Arduino Setup

This project also includes an **Arduino sketch** (`TrafficSign.ino`) that simulates the traffic light system using IR sensors and LEDs.  
It works alongside the Python desktop application to control real-time signal priorities.

### 🔌 Hardware Requirements:
- Arduino board (Uno or compatible)
- IR sensors
- LEDs (Red, Orange, Green)
- Resistors
- Jumper wires & Breadboard

### ▶️ How to Run:
1. Open the file [`TrafficSign.ino`](./TrafficSign.ino) in **Arduino IDE**.
2. Connect your Arduino board and upload the code.
3. Open the **Serial Monitor** at `9600 baud` to send/receive commands.
4. Use the Python GUI app (`main_gui_template.py`) to interact with the Arduino system.

👉 [Click here to view the Arduino code](./TrafficSign.ino)


## Support

#### For support or inquiries, please contact me at:
leemm2211@gmail.com

## Acknowledgements

***First and foremost**,* *we would like to thank God Almighty for granting us the strength and determination to complete this project.*

*We sincerely express our deepest gratitude and appreciation to **Dr. Jomana **, who supervised us during GP1. Her valuable guidance, consistent support, and encouragement laid the foundation for the successful progression of our project, We also extend our special thanks to **Dr. Shatha **, our supervisor during GP2, whose continuous guidance and valuable advice played a major role in enriching this project and helping us achieve the best possible outcomes. She was a true source of support and encouragement at every stage.*

*We also extend our heartfelt thanks to our families for their endless patience, encouragement, and prayers throughout the entire process, as well as to our friends who stood by us with their support and motivation.*

***Finally,*** *we believe this project was more than just an academic requirement; it was a valuable learning experience that broadened our knowledge, and we hope it will pave the way for more impactful and meaningful projects in the future.*


