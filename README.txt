1. Overview/description of the overall structure of the program (3 points) -> Game play
    The program was written in a very modular, logical and automatical way. Each job is a seperate function, and main can call
    those functions as wished to organize the gameflow in a clearer way. 

    Once the game starts, the main function shows the core structure of the game flow. FIrst, user will input their desire
    initial position, and AI will have it randomly. Next, user will input the move and step, which will be translated to 
    the code accordingly. Then AI will first calculate the optimized path to highest point grid at every step. A check for
    stuck-by-surrounding will be done after every player-AI step cycle. If one is stuck, the other can freely go until both
    become stuck, which ends the game with the output.
    PLEASE READ THE NOTE AT THE END.

2. Explains how to use pointers to construct the board (3 points) -> Game board
    The gameboard is constructed using malloc, allocating memories for a total of 37 individual spaces. Each space contains the
    struct Grid pointer, which represents a grid/ a location on the board itself. Each location is connected (left, down, 
    upright...) automatically using logics, not manually. 

    Player/AI can move by giving the current position, direction, number of step. Given the fact that the user input will always
    be of a same form, I determinded that the length of the input can help to determind the direction. Of course, there are some
    checks in place, and due to time constraint, I could not make it more restricted (but I can, given the time).



3. Explains how to implement the AI (3 points) -> Game AI
    A malloc is created of 24 spaces of memory. First 8 location represents the direction of AI, next 8 represents the max point
    on each direction, and last 8 is for number of steps to reach the destination.

    When checking each direction, assume the AI to move that direction, the checking will stop for that direction if the point
     on the current location is less than the max, because mathematically, once it becomes less than max value, there is not a 
     location on that direction that have a bigger point.

     Lastly, after a comparison, a direction and number of step is found, which will be encoded such that: it will be a 2-digit 
     int where digits represents steps and tens represents direction.

NOTE: Due to some crazy getline() input bug, everytime the player has to press ENTER before entering their move. Please 
accomodate this. Also, when the player gets stuck, the check code did not initiate (also due to the getline in my guess), 
so double eneter it somehow activates the check and ends the game, BUT, the score is affected by a 'space ASCII value, 
making the player score to become incorrect. I added -32 to indicate that I am aware of the error. 