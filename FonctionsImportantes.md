# Introduction #

Détail de l'utilisation de certaines classes / pseudoclasses / fonctions.

# Details #

Message :
Deux cas : on veut en envoyer un ou en recevoir un.
A peu près deux sous cas : par le prompt, ou par les socket.

  * Par le prompt : pas de traitement à faire.
  * Par la socket : il faut échapper les trucs.
On commence donc par appeler `mess__treatInput` sur les `char*` qui arrivent.

De même, il faut appeler `mess__treatOutput` sur les `char*` qui partent.

Ces fonctions s'occupent de virer / rajouter l'étoile à la fin, et d'échapper quand c'est nécessaire.

Ensuite, pour traiter le texte : `mess__parse`, qui renvoie un ptr sur struct Message qui contient toutes les infos nécessaires.

Pour envoyer les messages par les sockets, on les construit à la main en remplissant les champs comme on veut, puis on fait ` mess__toString ` (penser à appeler treatOutput).