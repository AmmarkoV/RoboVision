#!/bin/bash


#startup 
#lease dhcp 
#dhcpd

#manage partitions
#cfdisk

#create FileSystems and mount them afterwards
# root
#mkfs.ext4 /dev/sda1
# mount /dev/sda1 /mnt  

# boot
#mkfs.ext2 /dev/sda2
# mount /dev/sda2 /mnt/boot  

# home
#mkfs.ext4 /dev/sda3
# mount /dev/sda1 /mnt/home  

#install base system
#pacstrap /mnt base

#install GRUB
#pacstrap /mnt grub-bios

#generate fstab
#genfstab -U -p /mnt >> /mnt/etc/fstab

#change root
#arch-chroot /mnt

#hostname
#echo "hostname" > /etc/hostname

#Set timezone
#ln -s /usr/share/zoneinfo/Europe/Athens /etc/localtime

#modprobe dm_mod

#mkinitcpio -p linux
# grub-mkconfig -o /boot/grub/grub.cfg
# grub-install --directory=/usr/lib/grub/i386-pc --target=i386-pc --boot-directory=/boot --recheck --debug /dev/sda

#nano /etc/rc.conf

#exit
 
#SETUP IS COMPLETE :)


echo "Generating new PGP key for pacman .. please spam the keyboard to generate randomness.."
sleep 1
#pacman-key --init
echo "KEY IS READY , pleas stop spamming.."
sleep 5
#pacman-key --populate archlinux
#pacman -Syu

echo "X org "
pacman -S xorg-server xorg-xinit xorg-server-utils xf86-video-vesa
echo "3D stuff"
pacman -S mesa mesa-demos

echo "We have the following card ->"
lspci | grep VGA
pacman -Ss xf86-video | less
pacman -S xorg-twm xorg-xclock xterm
pacman -S ttf-dejavu

pacman -S lxde lxdm
echo "Please add lxdm as last service to autostart in /etc/rc.d"

pacman -S dbus
rc.d start dbus

pacman -S  sudo firefox evince file-roller net-tools vlc npapi-vlc-git flashplugin alsa-utils pulseaudio audacious audacity pidgin gnome-packagekit gimp simple-scan mumble pcmanfm leafpad transmission-gtk gpicview network-manager-applet networkmanager xfce4-notifyd hicolor-icon-theme gnome-icon-theme

groupadd networkmanager
echo " please run gpasswd -a “yourusername” networkmanager"

echo "@nm-applet" >> /etc/xdg/lxsession/LXDE/autostart
echo "@setxkbmap -option grp:switch,grp:alt_shift_toggle,grp_led:scroll us,gr" >> /etc/xdg/lxsession/LXDE/autostart

pacman -S wireless-tools rfkill
pacman -S xfburn

pacman -S git codeblocks sysprof