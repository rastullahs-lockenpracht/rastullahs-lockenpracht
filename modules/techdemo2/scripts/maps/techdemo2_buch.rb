#==================================
#		Tagebuch
#==================================
$tagebuchtexte = Hash.new
#$tagebuchtexte['name'] = ['titel','text'] 


#Hauptquest für alle

#Map1 laden
$tagebuchtexte['dorf1'] = ['Probleme mit Wilden Tieren I','Gestern bin ich mit einem Problem in einem der umliegenden Dörfer betraut worden. In einer kleinen Siedlung zwischen Wandleth und Ruchin haben die Menschen wohl Probleme mit Raubtieren.Die Dorferste, Sonja heißt sie, kam vor einigen Tagen persönlich bei uns vorbei und meinte, dies sei ein Fall für die Stadtwache. Meine Instruktionen sind klar: In das Dorf gehen und den Menschen helfen ihre Probleme in den Griff zu kriegen. Hoffentlich verlangt man von mir nicht einen Bogen in die Hand zu nehmen und Tiere zu jagen. Es ist jetzt früher Abend und ich sehe die Häuser schon. Ich sollte wohl zuerst Sonja aufsuchen um genau herausfinden was eigentlich los ist.']

#Erstes Gespräche mit Sonja
$tagebuchtexte['dorf2'] = ['Probleme mit Wilden Tieren II','Ich habe mit Sonja geredet. Um mehr über die Sache zu erfahren soll ich mit einigen Dorfbewohnern reden. Ihr Mann, Jarn, wohnt in ihrem Haus. Wolfram, ein Dichter, wohnt in einem zweiten und ein gewisser Tsafried hält sich anscheinend gerne in einer kleinen Baumgruppe in der Nähe auf. Wenn ich alle Informationen habe, die ich brauche soll ich wieder zu ihr zurückkehren und wir besprechen dann was zu tun ist. Scheinbar ist doch mehr an der Sache als einfach nur ein hungriges Tier.'] 

#Quest mit dem Reden geschafft. Questlistener?
$tagebuchtexte['dorf3'] = ['Probleme mit Wilden Tieren III','Ich habe mit allen dreien gesprochen. Das Bild fügt sich zusammen. Offenbar gibt es einen alten Mann, der im Wald lebt und manchmal mit den Dorfbewohnern Handel treibt. Dieser ist vor einer Woche verschwunden und niemand weiß wo er ist. Kurz danach haben die Übergriffe der Wölfe begonnen. Während Wolfram sich nur sicher ist, dass der Mann aus dem Wald ganz sicher mit den Wölfen im Dorf zu tun hat, sich aber nicht festlegen will ob er nun auch Opfer oder ob er Verursacher ist sieht Jarn die Sache eindeutig: Der Mann aus dem Wald hetzt die Wölfe gegen das Dorf, weil neidisch auf dessen Leben ist, besonders auf seine Frau Sonja, die mit dem Alten Mann Handel treibt und sich in einer überlegenen Situation zu befinden scheint. Sie gibt ihm wohl weniger als er verdient hat. Tsafried hingegen ist genau der anderen Meinung. Er ist von der Unschuld des Mannes überzeugt. Dieser scheint so etwas wie ein Mentor oder Lehrer für ihn zu sein und Tsafried macht sich große Sorgen, dass etwas im Wald passiert ist, dass den Mann abhält ins Dorf zu kommen. Das scheint auch irgend etwas mit dem auftauchen der Wölfe zu tun zu haben. Tsafried bittet mich im Wald nachzusehen. Nichts anderes habe ich vor! Ich sollte mit Sonja reden um weitere Schritte einzuleiten.'] 

#Hauptquests Spaltung
#Map2 laden
$tagebuchtexte['retten0'] = ['Der Turm','Der verlassene Turm, von dem mir erzählt wurde, ist ganz in der Nähe. Ich kann ihn über den Baumwipfeln schon sehen. Hier sollte ich wieder vorsichtiger sein, was auch immer auch für das Verschwinden verantwortlich ist, es könnte es auch auf mich abgesehen haben. Ich sollte mich etwas umschauen und nach dem Vermissten suchen.'] 
$tagebuchtexte['schnetzeln0'] = ['Der Turm','Der verlassene Turm, von dem mir erzählt wurde, ist ganz in der Nähe. Ich kann ihn über den Baumwipfeln schon sehen. Hier sollte ich wieder vorsichtiger sein. Wer weiß auf welche Weise mit der alte Kauz auflauern wird. Sobald ich ihn sehe werde ich ihn fangen und festsetzen. Das wichtigste ist jetzt die Sicherheit der Dorfbewohner, und die ist nur gewährleistet wenn der Alte Mann unschädlich gemacht worden ist.'] 

#Nach dem abschlußgespräch mit Sonja
$tagebuchtexte['retten1'] = ['Der alte Mann aus dem Wald I','Es ist eindeutig: Seit der alte Mann aus dem Wald hier nicht mehr gesehen wurde haben die Wolfübergriffe eingesetzt. Er muß sie vorher auf eine rechtschaffende Weise vom Dorf ferngehalten haben um die Bewohner zu schützen und nun ist etwas geschehen, was ihn davon abhält. Ich sollte in den Wald gehen und nach dem rechten sehen. Aber ich sollte vorsichtig sein, immerhin gibt es dort Wölfe, die nicht davor scheuen in ein Dorf voller Menschen zu kommen.'] 
$tagebuchtexte['schnetzeln1'] = ['Der alte Mann aus dem Wald I','Nun ist es raus: Der alte Mann aus dem Wald hetzt die Wölfe auf die Dorfbewohner! Allein diese Tatsache ist unheimlich genug und kann schon nicht mit den Zwölfen gefällen Dingen zugehen. Ich werde in den Wald gehen und ihn festsetzen, notfalls mit Gewalt. Dieser Verrückte ist eine Gefahr für die Öffentlichkeit.'] 

#druidentod.tb und waldwoelfe_retten.rb
$tagebuchtexte['retten2'] = ['Der alte Mann aus dem Wald II','Geschafft! Ein großer Wolf hatte den Vermissten angefallen und ihn im Turm festgesetzt, wo ich ihn verwundet und fiebrig fand. Ich musste den Wolf töten und habe den alten Mann notdürftig versorgt. Ich sollte nun ins Dorf zurückkehren und alles berichten, die Dorfbewohner können sich dann um alles weitere kümmern, nun wo der Weg wieder frei ist.'] 
$tagebuchtexte['schnetzeln2'] = ['Der alte Mann aus dem Wald II','Geschafft! Als ich auf ihn traf setzte er sich zur Wehr und ich mußte in kampfunfähig machen. So wird er zumindest niemanden mehr belästigen. Ich denke, nun wird sich der Wald, und damit auch die Wölfe, wieder beruhigen.'] 

#Nebenquests
#Wolfsrams Tür Dialog
$tagebuchtexte['wolfram'] = ['Wolfram öffnet die Tür nicht','Ich war an Wolframs Tür. Er öffnet mir nicht, aber ich hörte, dass er zuhause ist! Ich muß ihn dazu bringen mir die Tür zu öffnen.'] 

#Jarns Dialog
$tagebuchtexte['paket'] = ['"Ein seltsames Paket','Ich habe mich einverstanden erklärt ein Paket von Jarn zu Wolfram zu bringen. Dieser wohnt gleich gegenüber. Ich frage mich was dort drinnen ist. Waren das gerade Geräusche aus dem Inneneren?'] 

#Sonjas Dialog
$tagebuchtexte['aufenthalt'] = ['Aufenhaltsort des alten Mannes aus dem Wald','Zuletzt wurde der Mann in der Nähe eines verlassenen Turms gefunden. Sonja hat mir den Weg erklärt, ich muß einfach nur einem Pfad außerhalb des Dorfes folgen.'] 
