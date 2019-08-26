# Fast-Inverse-Square-Root

In 1999, id Software released the latest John Carmack offering: Quake III Arena. The game was innovative in a number of respects, but one real piece of ingenuity would only become apparent years later come 2005 when id Software open sourced the code online for miscreants and belligerants to pick apart and dissect.

To give some context, a major aspect of 3D scene rendering is computing the angles of incidence for things like lighting, shadows, and reflections. Part of this process is accomplished by computing vector normalisations, which requires that an inverse square root be taken.

Here, we will create a data visualisation that represents the relative performance gains of using the Q_rsqrt formula, in coparison to explicitly computing 1/sqrt(f).

I have included both the compiled and source C++ code that was used to compute our normalisations and their computation times, as well as all of the source images.

#### [Use nbviewier in order to see the animation, it does not render correctly within GitHub](https://nbviewer.jupyter.org/github/pboulos/Election-Retrospective/blob/master/OntarioElections.ipynb).

## Modules You'll Need to Clone 

It's easiest to clone this if you have Python installed within an Anaconda environment (ideally conda-forge). However, should you wish to have an explicit list of modules that you would need, here it is:

- pandas                    0.24.2 
- matplotlib                3.0.3
- seaborn                   0.9.0

It's possible that the notebook will function with versions lower than this, but for declarative clarity I opted to include the ones that the notebook was created on.

Please give me credit if you use any of these animations elsewhere!

The file you'll want to open is: FastInverseSquareRoot.ipynb
