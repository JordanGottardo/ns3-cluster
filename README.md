# Tesi Magistrale di Marco Romanelli
Tutti i documenti, file e script della mia tesi di laurea magistrale

## Organizzazione cartella
```
. Tesi - Romanelli
├── codice
├── data
├── doc
├── mappe
├── ns-3.26.zip
├── patch
├── scripts
└── README.md		
```
* _codice_: codice per le simulazioni con ns-3 (differenziati per i tre scenari).
* _data_: risultati delle simulazioni, in formato .csv.
* _doc_: tesi (LaTeX, pdf) e presentazione (Keynote, PowerPoint, pdf).
* _mappe_: mappe per ognugno degli scenari previsti.
* _ns-3.26.zip_: archivio contenente ns-3 (3.26) con tutte le modifiche presenti (modello a ostacoli con modifica 3D).
* _patch_: contiene alcune modifiche al codice ns-3: modello a ostacoli originale e modifica 3D, modifica a netanim.
* _scripts_: script in python e bash per la conversione dei file con SUMO.

## Prerequisiti
Per utilizzare il modello a ostacoli è necessario installare le CGAL (per maggiori informazioni leggere file _obstacle.srt_ all'interno del modulo _osbstacle_).
N.B.: se le librerie sono compilate dai sorgenti è necessario aggiungere i flag al compilatore e al linker in fase di configurazione di ns-3.

## Installazione
Per l'utilizzo del modello a ostacoli con ns-3 ci sono due modi.
Col primo, più veloce, basta estrarre dall'archivio _ns-3.26.zip_ il simulatore con il modello a ostacoli già presente,
poi basta aggiungere la configurazione dello scenario (cartella _codice/_) nella cartella _scratch_ di ns-3.
Per installare il modello su un'altra versione di ns-3 basta copiare il contenuto della cartella _3D Obstacle Model_ (_patch/ns-3_) all'interno dei sorgenti della versione di ns-3 utilizzata (in _src/_).

## INFO
Il materiale è disponibile anche nel mio repository all'indirizzo: https://gitlab.com/mromanelli/tesi

## Autore
Marco Romanelli, marco.romanelli.1@studenti.unipd.it
