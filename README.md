# Firefighter Pinball


## The game

Thank you for downloading!

This game is a pinball, made by two Videogame Design and Development students of the degree in CITM (UPC), based in the
[Firefighter Pinball](https://www.classicgame.com/game/Firefighter+Pinball).

You have 3 lives to reach the highscore. Good luck!

Enjoy!

## Controls and Debug

* Left and Right arrow keys to move the flippers.
* Down arrow key to shot the ball.
* Enter key to start the game and to pass from the end screen to the start screen.
* Escape key to close the game.
* F1 to see the degub stage, ball, flippers and sensors.

## Authors 

**David Valdivia Martínez:** [ValdiviaDev](https://github.com/ValdiviaDev)

**Aitor Vélez Tolosa:** [AitorVelez](https://github.com/AitorVelez)
 
## Credits

* Original Game and assets:[Firefighter Pinball](https://www.classicgame.com/game/Firefighter+Pinball) 


## Diferences with the original game

* There is a blue tube in the original game, which allows you to earn more points if the ball enters it. The tube does not appear in our game.
* The tube that teleports you to another level in the original game gives you a high amount of points in our version of the game.
* When the ball passes over the blue balls and medic crosses, they light up and remain like this until the player loses.
* Camera is static.
* No score multiplicator.
* The font used for the score is different in our version.
* Sensors that lift the ball up in the original game (at the sides of the flippers) just lift the ball up in our version, without
putting a barrier for the ball, like in the original game.
* A small amount of visual differences in the stage.

## Combo implementation

In the game there are to types of lights that light up when you pass them. If you touch all of the lights in the same
life, you will recieve a high amount of points. 500 points for the three medical crosses, and 1000 points for the blue
light balls.
The other bumpers give you a smaller amout of points, and if you arrive to the end of the stairway tube you will recieve 200 points every time you
arrive.

## Tools used
* c++ using Visual Studio 2017
* SDL Library
* Box2D Library
* [Vertex-Box2D-v1.3](https://github.com/d0n3val/Vertex-Box2D)

## License

MIT License

Copyright (c) 2018 David Valdivia Martínez and Aitor Vélez Tolosa

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Github repository link

https://github.com/ValdiviaDev/Firefighter-Pinball

