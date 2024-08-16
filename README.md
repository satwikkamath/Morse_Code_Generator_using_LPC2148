# **Morse Code Generator using LPC2148 and Proteus** 🤖🔌

## **Introduction** 🔍
The **Morse Code Generator** project aims to develop an embedded system that converts text input into Morse code signals. It utilizes the **LPC2148 microcontroller**, a **keypad** for text input, an **LCD** for displaying the input text and corresponding Morse code, and either an **LED** or **buzzer** for generating Morse code signals. 🎛️💡

## **Objectives** ✅
1. Interface a **keypad** with the LPC2148 microcontroller to input text characters in **Proteus** Software. 
2. Display the input text and corresponding Morse code on an **LCD**. 📺
3. Develop a software algorithm to convert text characters into Morse code accurately. 🔣➡️🔊
4. Generate Morse code signals through visual (**LED**) or audio (**buzzer**) outputs with correct timing for **dots**, **dashes**, and **spaces**. 🔴🔔
5. Implement **I2C protocol** to facilitate communication between the microcontroller and the LCD or other I2C-enabled peripherals. 🔗
6. Utilize **Keil uVision 4** for developing, debugging, and testing the microcontroller firmware to ensure the system functions correctly in real-time. 💻🐞

## **Components Required** 🛒
- **LPC2148 Microcontroller Development Board** 🖥️
- **Keypad** 🎹
- **LCD** (LM018L, LM016L, or I2C extender PCF8574A) 📺
- **LED** (RED) 🔴
- **Transistor** (BC547) 🔌
- **Resistors** (10K ohm) 🔌
- **Power Supply** (5.5V, 5V, and 3.3V) ⚡

## **Circuit Diagram** 🔌
![Circuit Diagram](https://github.com/user-attachments/assets/29d3538c-ca35-4956-a5d8-715953fbf65c)


## **Code** 💻
The project code is written in **C** and utilizes the **Keil uVision 4 IDE**. It includes functions for keypad reading, LCD initialization and control, I2C communication, Morse code conversion, and signal output. The main program integrates these modules to create the Morse code generator system.

You can find the complete code in the [code directory](https://github.com/shreeshatp83/MORSECODE/blob/main/morse_code.c). 📂


## **Applications** 🌐
1. **Communication**: The Morse code generator can be used for basic communication, especially in emergency situations or areas with limited infrastructure. 📡
2. **Education**: It can serve as an educational tool for learning Morse code and understanding its historical significance in communication. 🏫
3. **Signaling**: The system can be used for visual or auditory signaling purposes, such as in lighthouses, ships, or military applications. 🚢⚓

## **Result** 🎉
The Morse Code Generator project successfully converts text input into Morse code signals using the **LPC2148 microcontroller**, keypad, LCD, and LED/buzzer. The system accurately displays the input text and corresponding Morse code on the LCD while generating the appropriate signals. 🔊

## **Conclusion** 🏁
The Morse Code Generator project demonstrates the integration of various hardware components and software algorithms to create a functional embedded system. It showcases the capabilities of the **LPC2148 microcontroller** and the effectiveness of using **Proteus** for circuit design and simulation, along with Keil uVision 4 for firmware development and testing. 🤖🔬
