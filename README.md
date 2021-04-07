openbook-v2
===========

I wrote this in ~2005. I'm not aware of any earlier 3D interactive fiction interpreters...

Here's a 2013 [blog post](http://instantiatorgratification.blogspot.co.uk/2013/10/text-adventures-in-3d.html) on the topic.

It's written in C++ and relies on GLUT to do the rendering.

I'm using Frotz as the engine - as it's simple enough to bundle into the application with some minor adaptations.

![Screenshot of a game running](https://github.com/instantiator/openbook-v2/blob/master/docs/screenshots/openbook2/openbook2-spoiler10.PNG?raw=true)

The last time I had it running was probably around 2005, and it was working well then! The idea was to build it into an educational suite, and write accompanying games to meet targets on the national curriculum.

I ended up abandoning the project for my first full-time job. In the meantime, technology has grown and changed - and I suspect it would be far easier to rewrite the whole thing in a modern and platform-independent language.

The ultimate aim was to extend the Inform interpreter with features such as:

* Score keeping over time (and therefore identity management)
* Group sharing of scores (ie. for classes in schools)
* Time based assignments (ie. your homework for tonight is now unlocked)
* Awards (for completing tasks in the games, your rewards come with fun animations and go on your score board)

I was interested in building an attractive UI, and turning it into a fully working suite for schools. I'm opening up the source now, although I'd imagine there are better ways to solve the same problem now. There are plenty of Inform interpreters and far better 3D rendering engines to build on.
