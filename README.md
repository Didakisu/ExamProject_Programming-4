I was assigned to make the game Digger.

Controls:

-> In single-player mode the player moves with(WASD)/controller:(D-PAD) and shoots with (SPACE)/controller:(A);
-> In co-op mode:
		-Player1:same as the single player;
		-Player2:arrow keys/with controller is the same;
-> In versus mode one of the players plays as the player, the other one as the enemy;
-> With (F1) you can skip levels, (F2) mute sounds.

Game patterns used in this project:

-> Game Loop(Update method);
-> Component Pattern(for decoupling);
-> Dirty Flag(performance);
-> pimpl;
-> Command(actions as objects, decoupling input from game logic);
-> Observer and event queue pattern(decouple sender form receiver);
-> Threading(used for the sounds on very basic form);
-> Service Locator(for sounds, for easy access);
-> State(encapsulation);


https://github.com/Didakisu/ExamProject_Programming-4