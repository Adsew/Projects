echo Configuring phpmyadmin...
sudo rm -r /opt/bitnami/apps/phpmyadmin/conf/httpd-app.conf
sudo mv ./heatmap/install/httpd-app.conf /opt/bitnami/apps/phpmyadmin/conf/httpd-app.conf
sudo rm -r /opt/bitnami/mysql/my.cnf
sudo mv ./heatmap/install/my.cnf /opt/bitnami/mysql/my.cnf
sudo sudo chmod -R 777 /opt/bitnami/apps/phpmyadmin/htdocs/config.inc.php
sudo rm -r /opt/bitnami/apps/phpmyadmin/htdocs/config.inc.php
sudo mv ./heatmap/install/config.inc.php /opt/bitnami/apps/phpmyadmin/htdocs/config.inc.php
sudo sudo chmod -R 444 /opt/bitnami/apps/phpmyadmin/htdocs/config.inc.php
sudo rm -r ./heatmap/install
sudo ufw allow 3306
echo Creating application files...
sudo mv heatmap ./htdocs/heatmap
sudo /opt/bitnami/ctlscript.sh restart apache
sudo /opt/bitnami/ctlscript.sh restart apache
echo Application installation complete.
echo Restart the virtual machine for effects to finalize.
