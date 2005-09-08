
video = Video.new("Video/texture", "intro.ogv");
#video.addEventListener($VideoDisplay);
video.play();
$VideoDisplay.show(video.getTexture(), video.getTextureName());
