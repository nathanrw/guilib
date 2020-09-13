# guilib

This is (yet another) GUI library, primarily intended for games and other
graphical applications.

I want a UI library for hobbyist game development that is simple to embed in any
programming language, that is is platform agnostic, and is not dependent on any
particular rendering backend, input handling system or whatever. It should
follow a design that is easy to separate from the rest of the program, and state
should be explicitly managed: I am not overly fond of the IMGUI paradigm.

The design should be guided by the following principles:
- _Retained_ mode of operation. The call stack is not a GUI data structure.
- _Narrow API_ for ease of embedding.
- _No dependencies._ Provide abstractions where necessary which can be
  supplied by the caller.
  - But don't go crazy. Use of C++ and its standard library is OK internally.
- _Header-only_ for ease of integration. This allows the consumer to decide how
  the code is built.
- Functionality can be _limited_, but should be as _complete_ as possible.

The closest thing to what I want is `nuklear`, but I found the API to be too
wide for easy embedding, and I would prefer not to have the GUI so tightly
coupled to the code. This probably still doesn't justify rolling my own, but
there you go! I think it might also have had difficulties with game pads due
to its nature.

## The design so far

The key to easy integration is to have a _narrow API_. As much functionality as
possible should be made to fit general concepts that map easily to other
programming languages - and consumption of the interface that is exposed should
be as simple as possible.

What I've got at the moment is a hand-written C interface wrapping a C++
implementation. This means that the consumer's build system needs to know how to
build C++, but _not all of their calling code does_, which I think is the
important thing. This means that I can write code using the C++ standard
library, but the client doesn't necessarily need to know about it. A more purist
approach would use C all the way down as per nuklear, but I don't have the time
or the will.

The C interface exposes some _abstract interfaces_ using structs filled with
function pointer, as per the fairly well-known idiom. This allows the same
generic 'property-setting' interface to be use to configure widgets, and
potentially for data-binding - or at least that's the idea.

The general idea is that the application exposes a 'data model' by implementing
the property-getting-and-setting interface, and widget values are mapped to this
via some sort of expression evaluation.

The application thus doesn't need to talk to the API about what the GUI is
supposed to look like. It just presents data.

The interfaces themselves are specified either via a narrow API like so:

```
// psuedocode
widget_id create_widget();
properties* get_widget_properties(widget_id);
properties->set_int("position_x", 10);
```

or by providing a ui definition file like so:

```
// pseudocode with made-up properties
form inventory {
  title: "Inventory"
  resizable: "true"
  layout: "auto"
  grid items {
    binding: "player.items"
  }
}
```

leaving the library to sort out the details. This probably introduces as many
questions as it answers: how will the application control the appearance of the
UI? Who evaluates the expressions? What about when the values change? What about
a bidirectional mapping?

These are all things that need thinking about, but here are some vague answers:

1. The library will provide a sensible default appearance, but this will be
   customisable by the application. Also the *rendering* of the GUI is the
   responsibility of the application, although the library will provide as much
   help as possible. This might mean outputing 'drawing commands' as per
   nuklear, or it might mean something more abstract to facillitate custom
   animations, transitions and so on.
2. Expressions in the ui files that are supposed to resolved to data items are
   evaluated _by the application_, which supplies an interface implementation to
   do so. This should include the concept of 'compiling' an expression to
   provide the opportunity to avoid lots of repeated parsing.
3. The expression language is under the control of the application, so only it
   can truly know what needs updating - but it should be provided with a model
   for doing so; perhaps expressions could be queried as to whether they think
   they have changed.
4. A widget should be able to tell the application that it has been 'triggered'
   or edited, and supply the details. The application can then respond by
   updating its data. I think this should be conveyed as output from the
   function that supplies input to the UI, given the application control over
   what happens.

### Custom widgets

Can the application add new widget types or otherwise customise the behaviour of
widgets?

No.

If new types of widget are needed, they can be implemented in the library.

### Custom widgets - no, really?

I think there is one concession: the application might want to embed a 'graphics
area' or otherwise do some custom rendering to visualise a GUI element. For
instance, an inventory item might display a rotating 3d preview of a sword or
whatever.

I think I _definitely_ want to support this sort of custom rendering (somehow!)
but such widgets should not have custom behaviour. 

Or, mark out an area as unmanaged by the GUI, so input events in there will be
ignored, and where the application can do its own rendering.

I think these capabilities are actually quite important - this library is for
graphics-driven applications, and where else will you find a need for embedded
graphics?

### Generating GUI from data

If the application is exposing its data model, can we just generate GUI from it?

It would be good to have some form of this, e.g. for 'editing' GUIs. It would
look like so:

```
form editor {
  auto properties {
    binding: "selected_object.properties"
  }
}
```

The 'auto' widget constructs the necessary child widgets to handle editing all
of the data exposed by the property to which it is bound.

This would imply a need to add some extra metadata to properties to allow the
library to choose appropriate controls for editing them e.g. valid ranges.

### How is input going to work?

I imagine it will be similar to nuklear: you translate your framework's input
into something that the library can understand and pass it in. The output from
this will be a set of actions for the application to respond to.

It's not clear whether the library needs to know about _sound_, or if it can
simply tell the application 'you hovered over a button' allowing it to decide
what to do.

### How is styling going to work?

There should be a servicable 'default' style. It should be possible to provide a
'stylesheet' to customise the appearance of all widgets. It might be necessary
for very special styles for certain widgets (e.g. the big boss health bar...)
which would imply a need for widget 'classes' (as per css) as well as types. It
should _not_ be as powerful as css though.

Note also that because _rendering_ is provided by the application, it's not
clear how the 'stylesheet' should refer to outside assets. Nuklear for instance
has the concept of a 'texture', which allows you to smuggle in an id that can be
used by opengl later, without the library really knowing what it is.

So really how this is actually going to function depends on exactly how we do
rendering, what we delegate to the application, and how exactly we delegate it.

### How is rendering going to work?

Either output a sequence of 'draw commands' as per nuklear, or design something
more abstract that allows the application to, for instance, provide custom
transitions and animations. It might be that the drawing commands are the way to
go, and that there is a way to get sufficient information for the application to
make clever decisions in the draw command output.

There will need to be some conception of font rendering, which is a can of
worms, but must be doable. One lesson I learned writing a pygame binding for
nuklear is you really don't want a callback that is called once per character.
But overall, the graphics side of nuklear is very pleasant to use and should
serve as a benchmark.

Need to take care that there's not too much work being created for the
application. As I said with nuklear, it's quite pleasant in that regard.

### Another approach

What if I use SWIG? i.e. move away from header-only, write the C++ implementation
I was planning on writing anyway and use THAT as the interface through use of
SWIG to generate the wrapping?

## Building the example program

```
mkdir _build
cd _build
cmake ..
cd ..
cmake --build _build
```
 
