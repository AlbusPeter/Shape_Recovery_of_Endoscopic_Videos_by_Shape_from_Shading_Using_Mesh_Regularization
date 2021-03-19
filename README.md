# Shape-Recovery-of-Endoscopic-Videos-by-Shape-from-Shading-Using-Mesh-Regularization

Source Code for [Shape Recovery of Endoscopic Videos by Shape from Shading Using Mesh Regularization](https://albuspeter.github.io/projects/Shape-Recovery/icig_2017_sfs_mesh_deformation.pdf) <br> <br>

![showcase](https://github.com/AlbusPeter/AlbusPeter.github.io/blob/master/projects/Shape-Recovery/SFS.png)

OpenCV Version:2.4.11<br><br>

This is my very first project that I worked on during my junior Year. So here is my applogy if the code is not neat for you. The project aims to recover 3D surfaces from endoscopic videos using the simple Shape from Shading method. The raw results from Shape from Shading are noisy and unacceptable. We propose to utilize mesh regularizations to boost the Shape from Shading results. We also use `OpenGL` to visualize the final 3D reconstruction results, where we can change view angles and zoom in&out. <br><br>

# Note
-----
1. Make sure you have OpenCV and OpenGL environments set up.<br>
2. We utilized Matlab Engine for bilinear regression via sparse matrices. So make sure you set up Matlab engine as well.
3. If you have trouble extracting required data points, you can download here. ([rowpoints](https://drive.google.com/file/d/1_qHVf_1sVthQJFUmJ_D5jPsLB9D0sjHS/view?usp=sharing),[testpoints](https://drive.google.com/file/d/1aNigDMbydL_hFyIVjFrDn-jar0lgKALp/view?usp=sharing))<br><br>

# Change parameters for new videos
----
Since this is my first project, I did not have much experience in coding. I manually assigned several property parameters, such as the resolution of the video, and the number of frames. So you may need to change those parameters accoding to different videos. <br>
In `main.cpp`:<br>
1. The texture container variable declaration.
2. The `for` loop in `Read3DPoints()`.

In `Viewer.cpp`:<br>
1. The texture container variable declaration in `MyDisplay()`.
2. The texture container variable declaration in `Texinit()`.

In `Viewer.h`:<br>
1. The number of frames, `N`.

In `Draw.cpp`:<br>
1. The denominator in `DrawTriangles(int n)` should be `max(width,height)`.

For further questions, feel free to contact me at `albuspeter.rzh@gmail.com`.<br>

# Citation
If you find this useful in your research, please cite our paper "Shape Recovery of Endoscopic Videos by Shape from Shading Using Mesh Regularization" ([PDF](https://albuspeter.github.io/projects/Shape-Recovery/icig_2017_sfs_mesh_deformation.pdf)):
~~~
@inproceedings{ren2017shape,
  title={Shape Recovery of Endoscopic Videos by Shape from Shading Using Mesh Regularization},
  author={Ren, Zhihang and He, Tong and Peng, Lingbing and Liu, Shuaicheng and Zhu, Shuyuan and Zeng, Bing},
  booktitle={International Conference on Image and Graphics},
  pages={204--213},
  year={2017},
  organization={Springer}
}
~~~
