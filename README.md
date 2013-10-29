openbook-v2
===========

I wrote this almost a decade ago. It is the world's first (and now, extremely deprecated!) 3D interactive fiction interpreter...

Here's my recent [blog post](http://instantiatorgratification.blogspot.co.uk/2013/10/text-adventures-in-3d.html) on the topic.

It's written in C++ and relies on GLUT to do the rendering.

I'm using Frotz as the engine - as it's simple enough to bundle into the application with some minor adaptations.

![screenie](https://github.com/instantiator/openbook-v2/blob/master/docs/screenshots/openbook2/openbook2-spoiler10.PNG?raw=true)

The last time I had it running was probably around 2005, and it was working great then! The idea was to build it into an educational suite, and write accompanying games to meet targets on the national curriculum.

I ended up abandoning the project to go get a real job and do real stuff for other people. In the meantime, technology has grown and changed - and I suspect it would be far easier to rewrite the whole damn thing in a modern and platform independent language.

The ultimate aim was to extend the Inform interpreter with features such as:
* Score keeping over time (and therefore identity management)
* Group sharing of scores (ie. for classes in schools)
* Time based assignments (ie. your homework for tonight is now unlocked)
* Awards (for completing tasks in the games, your rewards come with fun animations and go on your score board)

There are plenty of Inform interpreters. I was interested in the UI and turning it into a fully working suite for schools. I'm opening up the source in case anybody wants to do anything with it, or take the project forward! If you do, contact me and let me know!

NB. I do not own copyrights on any of the libraries incorporated into this, or the textures I used (found on Google in 2005!). The same is true of the stories in the stories/ folder. I have included them there to help you find a subset of games that might actually work with it as it stands!
