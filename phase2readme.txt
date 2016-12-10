Phase 2

In this phase of the project I started off giving the warbird the ability to move foward and backward, 
as well as rotating about its axises. The program takes input from the user and the Warbird class records
the input to update the warbirds position on the next update. The warbird also now has warp capability.
After updating the warbirds movements I added a method in WarBird class called applygravity() which adds a negative vector to the warbird towards the
origin. From there I created a Missile class, a child of the Shape class. The missile class has a main update
method, three movement methods, a "find target" method, and many missile status methods that return a bool. The
main update method finds the target and/or calls one of the three movement methods. The missile movements are
either tracing the carrier, firing straight, or orienting toward the target. The missile can be a bit shaky at
times but usually finds the target if it is in radius. The Missile class also has a Target class which is 
made up of a vector of glm::mat4's (all of the missile's possible targets). Then I made a MissileSite class
which is similar to the WarBird class without all the additional movements. MissileSites just trace the planet
they are situated upon. Both the WarBird and MissileSites classes has a Missile class object. From there
I created a collision class which detects all the collision between obects. The speeds of the 
warbird are adjustable as well as the time quantums. Once the logic was completed I made new models to replace
my previous ones.

** I did not fix the orbits from the previous phase yet