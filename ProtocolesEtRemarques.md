# Details #

Contrôleur :
codé en C
- Un(e) thread/socket d'écoute qui ajoute des sockets de communication dans la liste
- thread de communication :  gestion séquentielle des communications grâce à la liste
- utiliser un itérateur "safe"


Routeur :
codé en C++
- possède une Map(tableau associatif) avec dest, next hop, dist (table de routage)
- on procède de façon séquentielle (parcours sur la liste des sockets, utilisation de "select")
- on protège la table de routage en écriture (mutex)
- on se connecte comme client sur un nouveau routeur apparu dans le réseau
- à la création, ajout des sockets comme étant serveur, ouverture des connections et attente