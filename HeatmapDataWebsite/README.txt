HOW TO INSTALL
-------------------------------------

1. Install Oracle VM VirtualBox
Link http://www.oracle.com/technetwork/server-storage/virtualbox/downloads/index.html

2. Download the recommended version of application bitnami LAMP
Link: https://bitnami.com/stack/lamp/virtual-machine

3. Open Oracle VM VirtualBox and drag the downloaded LAMP into the large white space
on the left of the window. On the import window, be sure to check refresh MAC addresses
on network adapters. This will allow the vm to give lamp a unique ip address.

4. Run the lamp stack and follow the instructions to create a new password. The current
default username and password are both bitnami. You must change the password to something
other than bitnami.

5. Enter the following commands to allow SSH to the lamp server.

sudo rm -f /etc/ssh/sshd_not_to_be_run

sudo ufw allow 22


sudo systemctl enable ssh

sudo systemctl start ssh



6. Now use the command 'sudo ifconfig' and take note of your ip address.

7. With SSH available, we now need to transfer the heatmap application included in this
bundle through ssh. For ease of use, we will use WinSCP to transfer files, but you can use
linux command shell as well should you know how.

Connect to the lamp server using the ip address noted in step 6. Use port 22 as we opened
in step 5. The username and password is 'bitnami' and 'yournewpassword' respectively, the
second being the password you set on lamp's first startup.

8. Once connected, drag the 'heatmap' folder included in this package to the lamp server.
This will move the folder into the home directory.

9. In lamp, while in the home directory still, you should see the folder when using the ls
command. Type the following commands.

sudo chmod 775 ./heatmap/install.sh
./heatmap/install.sh

This will run the installer and prepare both phpmyadmin and the heatmap app for use.

10. You can now access phpmyadmin and the heatmap from the browser. Before that, restart
the lamp stack. Before loging in, there should be a default password written in the yellow
text that looks like a jumble of characters (Eg."YnvDa4Dndsa"). Note the password then login
as normal.

After a few moments to allow the server to start, open a browser and access ip/phpmyadmin in
the URL bar and replay ip with the ip of your lamp server.

Log in to phpmyadmin using root as the username and the password you just noted for the password.
It is possible you may need to use the password you set for the bitnami login.

Go to "User accounts" and click edit privileges for the root user. At the top, click
"Change password" and set the password to no password (a radio button above the fields to enter
a new password. If you wish to set a password, you will have to change the login password variable
on the website heatmap.php file later to match.

Return to "User accounts" and create a new user. This user will be used by the clicking program.
Name the user and give it a password (not no password). Provide the user with at least all "Data"
and "Structure" privileges.


RUNNING THE APPLICATIONS
-----------------------------------------

	C++ Click Application

Open the ChidozieJamesA3 solution included in the package. Compile and run the application.
Enter in the ip of the server with the port 3306 (by default for sql) and the username and password
of your new user account as prompted. When the program connects, click around on the map to send
points to the server.

	Heatmap Website

Now we can open the heatmap website. Open the browser and enter ip/heatmap/heatmap.php into the
URL bar. The website will access the server and draw the heat on the map where the points were clicked.
If you changed the root password to something other than none, it will fail to authenticate.

Sometimes the map image will fail to load, simply refresh the page and it will show.

If you have not yet used the click application, there will be no data on the server. In this case, the
website will inform you it can not find the heatmap database. Simply make some clicks in the application
and refresh the website.

