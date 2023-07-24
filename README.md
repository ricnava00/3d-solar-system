# 3D Solar System Viewer

<div style="overflow: hidden;">

![solar_system](https://i.ibb.co/f0vKMRp/solar-system-cover.jpg)

</div>

## About

<div style="display: flex;">

<img align="left" width="300" src="https://i.ibb.co/w734hKz/3d-solar-system-logo.jpg"/>

<div style = "margin: auto; overflow: hidden;">

<p align = "justify">

**3D Solar System Viewer** is a Vulkan application born from the idea of creating a tool for educational purposes, able to perform what is usually impossible to do on a screen: allow the user to have an idea of ​​the scale of the planets, the distances and the movements they perform.  The large differences between planets and celestial motions prevent everything from being represented on the same spatial and temporal scale. To overcome this limitation, the user can decide to focus on particular planets and characteristics of the solar system.

</p>

</div>

</div>

<br/>

## Usage
The user can interact with the application through the following commands:

• `A`:&nbsp;&nbsp;&nbsp;&nbsp;Move left<br/>
• `D`:&nbsp;&nbsp;&nbsp;&nbsp;Move right<br/>
• `W`:&nbsp;&nbsp;&nbsp;&nbsp;Move forward<br/>
• `S`:&nbsp;&nbsp;&nbsp;&nbsp;Move backward<br/>
• `R`:&nbsp;&nbsp;&nbsp;&nbsp;Move up<br/>
• `F`:&nbsp;&nbsp;&nbsp;&nbsp;Move down<br/>
The above movements can also be performed using the **left stick of the gamepad**.

• `←`:&nbsp;&nbsp;&nbsp;&nbsp;Rotate left<br/>
• `→`:&nbsp;&nbsp;&nbsp;&nbsp;Rotate right<br/>
• `↑`:&nbsp;&nbsp;&nbsp;&nbsp;Rotate up<br/>
• `↓`:&nbsp;&nbsp;&nbsp;&nbsp;Rotate down<br/>
• `Q`:&nbsp;&nbsp;&nbsp;&nbsp;Rotate clockwise<br/>
• `E`:&nbsp;&nbsp;&nbsp;&nbsp;Rotate counterclockwise<br/>
The above movements can also be performed using the **right stick of the gamepad**.

Additional actions can be performed using the appropriate buttons.

## Build
On UNIX-like systems, the application can be built using the given makefile:
```console
foo@bar:~$ make
```
To build and run:
```console
foo@bar:~$ make run
```
For frequent testing, it is recommended to keep the development environment clean:
```console
foo@bar:~$ make run clean
```

## Contribution
The project has been developed by [Alberto Boffi](https://github.com/albertoboffi) and [Riccardo Nava](https://github.com/ricnava00) as project of the Computer Graphics Master's course at Politecnico di Milano.

A special thanks goes to [Grecya D'Angiò](https://github.com/grecyadangio) for providing the data on the planets, the creation of the logo and for many useful tips.

## License
This project falls under the MIT license.

Texture Credit: [Wikimedia Commons](https://commons.wikimedia.org/wiki/Main_Page)