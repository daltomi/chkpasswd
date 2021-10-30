# chkpasswd

## Purpose:
Verify that the password of a particular user is not being used.
If yes, it is informed by a message and notified on the desk.

<img src="https://github.com/daltomi/chkpasswd/raw/master/screenshots/scr0.png"/>

<img src="https://github.com/daltomi/chkpasswd/raw/master/screenshots/scr1.png"/>

#### User and password:

The default password is `toor` for chkpasswd.desktop

The username is automatically detected, therefore, by default
the username is the one that executes the application and the same for
the title, this avoids the use of `whoami` in the parameter line.


#### "Change password ..." button

Only if your notification agent support it.

When you click on the button, you try to open the _User Manager and
Groups_, which will depend on the current desktop (XFCE, MATE, etc).

### Usage:

```bash
/usr/bin/chkpasswd
```

It supports the following parameters:

`-u`  to indicate the name of **user**. By default automatically detected.

`-p`  to indicate the user **password**.

`-m`  to indicate the informational **message**. By default "Please, you must change the password."

        Note: for line breaks use the following mode:
        `-m` "Start the message"\n"Another line"

`-t` to indicate the **title** of the message.

`-a` to indicate the **alarm**, `seconds:repeat`

`-b` **not** show action **button**, __"Change password ..."__

`-g` name of the **program** to _User Manager and Groups_`.

`-s` use the username as a password (overwrite the option -p).

`-h` shows a summary of help.


**Examples:**


- Current user with a password:

```bash
chkpasswd -p toor
```
In case the current user uses the password **toor** it will show a
alert message, otherwise no message will be displayed.



- Some user with a password:

```bash
chkpasswd -u mike -p toor
```


- Alarm

Repeat the message 8 times every 60 seconds.

```bash
chkpasswd -p toor -a 60:8
```

Repeat only once in 8 seconds.

```bash
chkpasswd -p toor -a 8
```


### Depedencies:

- GCC >=5, supports C99
- glib2
- libpam
- libnotify
- pkg-config

### Compile:

```bash
make
```

### Install:

Two files are installed, `chkpasswd` in `/usr/bin` and the file `chkpasswd.desktop` in
`/etc/xdg/autostart`.

```bash
make install
```


##### Users and Groups Administrators:
- MATE: mate-system-tools (mate-users-admin)
- XFCE: mate-system-tools (mate-users-admin)
- GNOME: gnome-system-tools (users-admin)


### .desktop file:
If the user password has special characters (#%? ...) use the
Escape sequence: `\`.
For example, for the password `Yu%?` You should use the sequence of
escape, like this: `Yu\%\?`

