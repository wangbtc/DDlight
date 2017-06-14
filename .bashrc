# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
[ -z "$PS1" ] && return

# don't put duplicate lines in the history. See bash(1) for more options
# don't overwrite GNU Midnight Commander's setting of `ignorespace'.
export HISTCONTROL=$HISTCONTROL${HISTCONTROL+,}ignoredups
# ... or force ignoredups and ignorespace
export HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "$debian_chroot" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

#if [ -f ~/.bash_aliases ]; then
#    . ~/.bash_aliases
#fi

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    eval "`dircolors -b`"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    #alias grep='grep --color=auto'
    #alias fgrep='fgrep --color=auto'
    #alias egrep='egrep --color=auto'
fi

# some more ls aliases
#alias ll='ls -l'
#alias la='ls -A'
#alias l='ls -CF'



# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
fi




alias rmtilde='rm *~'
alias ew='emacs -nw'
alias mp='mplayer -nosound'
alias lr='ls -alrth'
alias lrt='ls -lrthiG|tail -n 15'
#alias sr='source ~/bin/sourceroot.sh'
#alias sr='source ~/bin/root-6.02.02/config/thisroot.sh'
alias ssh='ssh -Y'
alias rsp='source ~/remove_svn_pass.sh'
alias qs='ssh s3 qstat  -u penning'
alias tier3='ssh  -o ServerAliveInterval=300 tier3'
alias tier1='ssh  -o ServerAliveInterval=300 tier1'
#alias convert='/Users/penning/bin/ImageMagick-6.9.0/bin/convert'
alias lxplus='ssh  -oPubkeyAuthentication=no -l penning -o ServerAliveInterval=300 lxplus.cern.ch'
export SVN_EDITOR='emacs -nw'

YELLOW="\[\033[0;33m\]"
GREEN="\[\033[0;32m\]"
RESET='\[\e[0m\]'
#PS1="\@ \u@\h:$GREEN\w$RESET >$"
PS1="$YELLOW\h:$GREEN\w$RESET>"
#export LD_LIBRARY_PATH=/Users/penning/Desktop/hepevtMC/x86_64-mac106-gcc42-opt/lib/lib:$LD_LIBRARY_PATH
#export MAGICK_HOME="$HOME/bin/ImageMagick-6.7.1"
#export PATH="$MAGICK_HOME/bin:$PATH"
#export DYLD_LIBRARY_PATH="$MAGICK_HOME/lib/"



#export LD_LIBRARY_PATH=$ROOTSYS/lib:$PYTHONDIR/lib:$LD_LIBRARY_PATH
#export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH
#export MAGICK_HOME="/Users/penning/bin/ImageMagick-6.9.0" && export PATH="$MAGICK_HOME/bin:$PATH" && export DYLD_LIBRARY_PATH="$MAGICK_HOME/lib"



HISTSIZE=100000
