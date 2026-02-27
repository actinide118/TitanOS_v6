# Dépendances
Pour faire tourner ce programme vous aurez besoin des outils du paquet `qemu` , `gcc` et `nasm`

# Compilation
Après avoir satisfait les dépendances éxecuter `make build`

# Éxecution
Éxecuter `make all` ou `make run`

# Résultat
Écran d'acceuil:

<img width="730" height="463" alt="titanOS1" src="https://github.com/user-attachments/assets/f6a70591-013e-4589-a29c-80e45694a97d" />

<img width="730" height="463" alt="titanOS2" src="https://github.com/user-attachments/assets/3feff538-8fbd-46b9-b58a-03676aa9b60c" />

# Commandes
### end
Arrête le cpu mais n'éteint pas la carte graphique
### clear 
Efface l'écran
### version
Ecris la version
### read
Affiche sous forme hexadécimal un secteur disque
### tick
Renvois le nombre de tick cpu depuis l'nitialisation de l'horloge
### cat 
Est sensé afficher sous forme textuel un secteur disque
### page
Donne une plage d'adresse dispnible ( ceci ne permet pas d'isolation mémoire car un programme, si il y avait un moyen d'en éxecuter, peut accéder toute la mémoire )
### int 
Cause plus ou moins un plantage du ystème ( voulu )
### 2Dtest
Affiche une "image" composée de deux éléments distint
### hist 
Ne marche pas.
### fat_format
Inutile ( je ne suis même pas sur qu'il fonctionne )