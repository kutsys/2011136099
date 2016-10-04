# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
# $- : 현재 셸 옵션을 저장하는 환경변수다. 옵션의 값은 아래와 같다.
# h: hash all, i: interactive, m: monitor, 
# B: brace expand, H: hist [history] expand, 
# e: errexit, C: noclobber
# 위 case 문은 $- 변수값(문자열) 중 문자 i가 포함되었는지 확인한다. 
# i 값은 현재 셸의 interactive 여부를 뜻한다.
# 그리고 포함이 되어있다면 다음 스크립트를 실행하고 그렇지 않다면 현재 실행중인 .bashrc 스크립트를 종료한다.
# 즉, 현재 셸이 interactive 하지 못하면 스크립트를 종료한다.
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
# HISTCONTROL 변수를 ignoreboth 값으로 설정한다.
# HISTCONTROL 변수는 중복되는 명령에 대한 기록 여부를 저장하는 변수다.
# HISTCONTROL 변수가 가질 수 있는 값은 다음과 같다.
#   1. ignorespace: 스페이스 공백으로 시작되는 명령은 무시한다.
#   2. ignoredups: 중복되는 명령은 무시한다.
#   3. ignoreboth: 위 1, 2 두 경우 모두 무시한다.
# HISTCONTROL 변수를 ignoreboth 값으로 설정했으므로
# 명령어 history에서 스페이스 공백으로 시작되었던 명령어와 중복된 명령어는 찾을 수 없다.
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
# shopt는 shell option의 약자다. 
# 명령어 shopt를 사용해서 현재 옵션 전체를 조회할 수 있다.  
# -s옵션은 뒤에 나올 설정을 set한다는 것이다. 
# 즉 위 코드에서는 셸 옵션 중 histappend 설정을 set한다는 것이다. 
# 만일 histappend 옵션이 unset되어 있다면 
# history file에 새로운 항목을 덧붙여 추가하는 것이 아니라, 이전 내용에 덮어쓴다.
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
# HISTSIZE는 BASH 세션이 진행되는 동안 메모리에 저장되는
# history list의 명령어 수다.
# HISTSIZE를 1000으로 설정하였다.
HISTSIZE=1000
# HISTFILESIZE는 BASH 세션을 시작하거나,
# 끝날때 참조하는 history file의 명령어 항목 수다.
# HISTFILESIZE를 2000으로 설정하였다.
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
# shopt 명령어를 통해 checkwinsize 옵션을 set하였다.
# 이는 Bash가 각 명령어가 끝났을때 필요하면
# 터미널 windows의 LINES와 COLUMNS를 받아와 업데이트 할 수 있다는 옵션이다.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
# shopt 명령어를 통해 globstar를 set하게되면 경로에서 
# "**"을 사용하여 확장 글로빙을 할 수 있다.
# 이를 사용하면 모든 파일과 0개 이상의 디렉토리과 그 하위 디렉토리와 매칭된다.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
# [ 은 test와 같은 기능을 하는 명령어다.
# 가독성을 위해 [ 명령을 사용하는 경우에 ]를 덧붙인다.
# [ 의 -x 옵션은 /usr/bin/lesspipe가 실행가능한 파일이면 참을 반환한다.
# &&은 AND 목록이다.
# AND 목록은 일련의 명령들을 수행할 때 사용한다.
# 한 명령이 참이면 차례로 다음 명령을 수행하고 거짓인 명령이 있으면 그 즉시 수행을 끝낸다.
# eval은 갈무리된 명령결과로 반환된 위 두 개의 명령(export로 시작하는)을 실행한다.
# LESSOPEN과 LESSCLOSE 환경변수를 설정한다.
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
# [의 -z 옵션은 뒤에 나오는 string(문자열)이 널이면 참을 반환한다.
# ${debian_chroot:-}은 매개변수 치환 구문이다.
# debian_chroot가 널이면 -뒤에 나오는 값으로 치환한다.
# 즉 debian_chroot 변수가 널이고
# /etc/debian_chroot 파일을 읽을 수 있다면
# debian_chroot의 값을 /etc/debian_chroot 파일 내용으로 설정한다.
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
# 환경변수 $TERM은 터미널의 종류를 저장한다.
# '*'은 와일드카드 확장으로 임의의 문자열에 부합한다.
# 위 case 문은 환경변수 $TERM의 값이 xterm-color 이거나 -256color로 끝나면
# color_prompt 변수를 yes로 설정한다.
case "$TERM" in
    xterm-color|*-256color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

# /dev/null은 범용 "비트 휴지통"이다.
# - 위 코드는 force_color_prompt가 널이 아니고
# /usr/bin/tput이 실행가능한 파일이면
# tput setaf 1 명령의 출력 내용을 범용 비트 휴지통에 모두 버린 후
# color_prompt 변수를 yes로 설정한다.
# 만일 /usr/bin/tput이 실행가능하지 않거나 존재하지 않다면
# color_prompt의 변수를 비워둔다.
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

# 변수 color_prompt의 값이 yes면
# 1차 명령 프롬프트 변수인 PS1을 위 코드에 나온 형식으로 지정해준다.
if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
# 변수 color_prompt의 값이 yes가 아니면 PS1을 else 이후에 나온 형식으로 지정해준다.
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
# unset 명령은 현재 셸 환경에서 변수나 함수를 제거한다.
# 즉, color_prompt 변수와 force_color_prompt 변수를 제거한다.
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
# 위 case 문은 환경변수 $TERM의 값이 xterm으로 시작하거나
# rxvt로 시작하면 $PS1 변수를 다음과 같이 설정하라는 것이다.
# 그 이외의 경우에는 아무일도 하지 않는다.
# '* )': 와일드카드를 사용하여 그 외의 모든 경우를 나타내고 있다.
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
# ls와 grep등의 color 지원을 alias명령어를 통해 설정하고 있다.
# alias는 자주 사용하는 명령어를 간편하게 사용하기 위해 특정문자로 설정해주는 명령어다.
# 위 if문을 보면 /usr/bin/dircolors가 실행 가능한 파일이면 다음을 수행한다.
# ~/.dircolors 파일이 읽을 수 있는지 확인한다.
# 읽을 수 있다면 eval 명령어를 통해서 dircolors -b ~/.dircolors의 결과를 실행한다.
# ~/.dircolors 파일이 읽을 수 없거나 존재하지 않으면 dircolors -b의 결과를 실행한다.
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    
# alias 명령어를 이용해
# ls, dir, vdir, grep, fgrep, egrep의 color 지원을 활성화한다.
    
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# colored GCC warnings and errors
# GCC의 warnings과 errors에 대해 color지원을 하고 싶다면
# 그 아래의 내용을 주석처리 하지 않는다.
#export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
# alias 명령어를 통해 ls명령어에 관한 추가적인 간편 설정을 한다.
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
# -u, --urgency는 긴급함의 정도를 나타낸다 (low, normal, critical)

# -i "$([ $? = 0 ] && echo terminal || echo error)"
# -i, --icon=ICON[,ICON…]: 화면에 표시할 아이콘 파일을 지정한다.
# $?은 최근 실행시킨 프로그램의 종료 값을 저장하는 환경변수다.
# 즉, exit code가 0이면 terminal을 출력하고 그렇지 않으면 error를 출력한다.

# $(history|tail -n1|sed -e '\''s/^\s[0-9]+\s//;s/[;&|]\s*alert$//'\'')
# history|tail -n1은 history에 있는 마지막 커맨드를 반환한다.
# sed -e '\''s/^\s[0-9]+\s//;s/[;&|]\s*alert$//'\'')는 두 개의파트로 구분할 수 있다.
# 3.1 sed 's/^\s[0-9]+\s//': 
# 앞에 나오는 공백과 탭을 제거하고 모든 숫자를 제거한다. 
# 그리고 뒤에 나오는 공백과 탭을 제거한다.
# 3.2 s/[;&|]\s*alert$//: 
# 앞에 나오는 ';', '&', '|', 탭과 공백을 제거한다. 단어 "alert"도 제거한다.
# 이는 마지막 실행한 명령어를 깨끗하게 만들기 위한 것이다.
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

# [ 의 -f 옵션은 file이 정규적인 파일이면 참이다.
# '.(마침표)명령'은 현재 셸에서 명령을 수행하게 한다. 
# 일반적으로 스크립트에서 외부 명령이나 외부 스크립트를 수행하는 경우 하위 셸에서 실행된다.
# 그러나 마침표 명령을 사용하면 호출한 스크립트와 같은 셸 안에서 수행된다.
# 즉, ~/.bash_aliases가 정규적인 파일이면 현재 셸에서 ~/.bash_aliases를 실행한다.
if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).

# shopt 명령어의 -o 옵션은 뒤에오는 optname의 설정 값을 확인할 수 있다.
# -q 옵션은 출력하지 않는다.

# 즉, shopt의 posix가 off라면 다음을 시행한다.
# - /usr/share/bash-completion/bash_completion이 정규적인 파일이면
# 현재 셸에서 /usr/share/bash-completion/bash_completion을 실행한다.
# 만일 그렇지 않고 /etc/bash_completion가 정규적인 파일이면
# 현재 셸에서 /etc/bash_completion을 실행한다.

if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi
