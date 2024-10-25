<a id="readme-top"></a>

<br />
<div align="center">

  <h3 align="center">Image Processor</h3>

  <p align="center">
    An Image Processing application featuring filters and ASCII art generation in a GUI.
    <br />
    <br />
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Image processor is a program written in C++ using the Qt framework based around (as the title suggests) processing images. The functionalities are split into the ASCII art generator which generates ASCII art by taking in an image as input and filters which apply a filter to an image.

This project was created and submitted for the final project of Harvard's CS50 Introduction to Computer Science.

It was originally added to my secondary GitHub account.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

For this project, the Qt framework together with Qt Creator were used in order to create the graphical user interface.

* [![Qt][Qt-logo]][Qt-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

The two primary functions of this program are the ASCII Art Generator and the Filters either of which can be toggled using the buttons near the top of the window.

### ASCII Art Generator

![Image of ascii art generator page](/images/ProjectScreenshot3.png)

In order to generate ASCII art the user will need to upload an image of the supported file types.

By default, the size of the art created will be 100 characters wide however the user will be able to input any size between 1 and 1000. It should be noted that size in this context refers to the number of characters that will be used for the tallest side of the image. Additionally, since characters are taller than there are wide, there are half as many characters on the vertical axis.

Once the image has been loaded successfully, it should look something like this:

![Image of ascii art generator page with loaded duck](/images/ProjectScreenshot4.png)

### Filters

![Image of filters page](/images/ProjectScreenshot.png)

Similar to the ascii art generator, the user needs to first upload an image of the supported file types before being able to do anything.

Once an image has been added, a preview will be displayed. There are 5 different filters to choose that can be toggled as well as two sliders that apply for certain filters to adjust the "intensity" of the filter on the image.

The user also has the option to download the image with the applied filter through the `Download Image` button.

Below is an image of the CS50 duck with the black and white filter applied with maximum intensity.

![Image of filters black and white duck](/images/ProjectScreenshot2.png)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Project Link: [https://github.com/Joseph-RF/ImageProcessor](https://github.com/Joseph-RF/ImageProcessor)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Find below a list of resources I found helpful during the development of this project.

* [Qt Documentation](https://doc.qt.io/)
* [ProgrammingKnowledge's Qt Youtube Tutorial](https://www.youtube.com/watch?v=EkjaiDsiM-Q&list=PLS1QulWo1RIZiBcTr5urECberTITj7gjA&index=1t)
* [Qt Examples and Tutorials](https://doc.qt.io/qt-6/qtexamplesandtutorials.html) Although these are already in the documentation linked above I wanted to draw attention to examples and tutorials. I found them quite useful.
* [Badges for README](https://github.com/alexandresanlim/Badges4-README.md-Profile)
* [README Template](https://github.com/othneildrew/Best-README-Template)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[Qt-url]: https://www.qt.io/
[Qt-logo]: https://img.shields.io/badge/Qt-41CD52?style=for-the-badge&logo=qt&logoColor=white
