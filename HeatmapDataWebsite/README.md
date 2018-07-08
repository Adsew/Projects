## Heatmap Data Collection In Games

This was created in a two person team with Chidozie Regis Ikpe

# Warning, Large size due to BOOST

This project is a simulation of data collection in video games to record location data on maps during playtesting.

There is two parts to this project;

* A C++ local program that records clicks made on an image and sends that location data to a server using mySQLConnector (hence why boost is required)
* A website residing on a Bitnami LAMP server that reads the collective location data from the phpmyadmin mySQL database on the same server and displays a coloured heatmap.

The instalation instructions are in the README.txt file, however due to the complex nature of the server that was used, you can review the code we created in the following locations:

* heatmap/heatmap.php for the website code, including sql connection code and coloured heatmap generation based off that data.
* heatmap/HeatMapStyle.png for an example of the heatmap colouring used.
* heatmap/install.sh for the shell script used to install part of the server.

* ChidozieJamesA3\ChidozieJamesA3 for the C++ code for the local location data simulator.

The program runs in both 32 and 64 bit.
