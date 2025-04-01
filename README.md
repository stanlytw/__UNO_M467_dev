- [NuMaker UNO board package for Arduino IDE](#numaker-uno-board-package-for-arduino-ide)
- [Supported Boards & Features](#supported-boards--features)
- [Board Package Installation Steps](#board-package-installation-steps)
- [Disclaimer](#disclaimer)
- [Important Notice](#important-notice)

# NuMaker UNO board package for Arduino IDE
This repository contains the package descriptor for NuMaker UNO boards

# Supported Boards & Features

| NuMaker Boards | Rich GPIO | Embedded SD |Embedded CAN/FD<br>nvtCAN(*Note1)  | RS485 | Ethernet | Firmware Upload<br>by ISP(*Note2) |
| -------------- | ---------------- | ------|----------------------------------------------- |------|----|----|
| **[NuMaker-IOT-M467](https://direct.nuvoton.com/en/numaker-iot-m467)**       | ✔️  | ✔️| ✔️| ✔️| ✔️ | ✔️                                            ||
| NuMaker-M467SJ_SD<br>(*Note3)       |✔️              | ✔️                              | ❌  | ✔️| ✔️| ✔️| ✔️                               ||
| NuMaker-M467SJ_CAN<br>(*Note3)      |✔️              | ❌                              | ✔️  | ✔️| ✔️| ✔️ | ✔️                               ||

Note1: Compatible to [Seeed_Arduino_CAN](https://github.com/Seeed-Studio/Seeed_Arduino_CAN)

Note2: Require dedicated loader and application code.

Note3: It would be commercial available soon.

# Board Package Installation Steps
1. Download and Install Arduino IDE from https://www.arduino.cc/en/Main/Software
2. Double-click arduino.exe, and then go to **File->Preferences**
3. Paste following URL to 'Additional Boards Manager URLs' input field: 
`https://raw.githubusercontent.com/OpenNuvoton/NuMaker_UNO/master/package_nuvoton_index.json`
4. Under **Tools->Board->Boards Manger**, search `NuMaker UNO`, click **Install**
5. You can now select NuMaker UNO and install it in Arduino IDE.

# Compatible Versions of Arduino IDE
* 2.0.0 up

# Disclaimer
The Software is furnished "AS IS", without warranty as to performance or results, and
the entire risk as to performance or results is assumed by YOU. Nuvoton disclaims all
warranties, express, implied or otherwise, with regard to the Software, its use, or
operation, including without limitation any and all warranties of merchantability, fitness
for a particular purpose, and non-infringement of intellectual property rights.

# Important Notice
Nuvoton Products are neither intended nor warranted for usage in systems or equipment,
any malfunction or failure of which may cause loss of human life, bodily injury or severe
property damage. Such applications are deemed, "Insecure Usage".

Insecure usage includes, but is not limited to: equipment for surgical implementation,
atomic energy control instruments, airplane or spaceship instruments, the control or
operation of dynamic, brake or safety systems designed for vehicular use, traffic signal
instruments, all types of safety devices, and other applications intended to support or
sustain life.

All Insecure Usage shall be made at customer's risk, and in the event that third parties
lay claims to Nuvoton as a result of customer's Insecure Usage, customer shall indemnify
the damages and liabilities thus incurred by Nuvoton.

Please note that all data and specifications are subject to change without notice. All the
trademarks of products and companies mentioned in this datasheet belong to their respective
owners.


**Copyright (C) 2023 Nuvoton Technology Corp. All rights reserved**
