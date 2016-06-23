mv ../build/out.h5 output
cd output
h5topng -t 0:999 -Zc dkbluered -a yarg  out.h5
ffmpeg -i out.t%03d.png out.mp4 -y
vlc out.mp4
