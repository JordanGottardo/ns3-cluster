# Tesi Magistrale di Jordan Gottardo
Tutti i documenti, file e script della mia tesi di laurea magistrale

## Organizzazione cartella
```
. Tesi
├── Presentazioni
├── gridJobsTemplate
├── jobsTemplate
├── libs
├── maps
├── ns-3.26
├── scripts
└── tesi	
```
* _Presentazioni_: Presentazioni varie.
* _gridJobsTemplate_: template per lanciare i job per lo scenario Grid.
* _jobsTemplate_: template per lanciare i job degli scenari Padova e LA.
* _libs_: librerie e dipendenze varie.
* _maps_: mappe dei vari scenari.
* _ns-3.26_: ns-3 (3.26) con tutte le modifiche presenti (modello a ostacoli con modifica 3D e implementazione degli algoritmi FastBroadcast e ROFF).
* _scripts_: script vari (conversione mappe, print grafici)
* _tesi_: tesi

## Prerequisiti
Per utilizzare il modello a ostacoli è necessario installare le CGAL (per maggiori informazioni leggere file _obstacle.srt_ all'interno del modulo _osbstacle_).
N.B.: se le librerie sono compilate dai sorgenti è necessario aggiungere i flag al compilatore e al linker in fase di configurazione di ns-3.

## Installazione
Utilizzare il simulatore contenuto in ns-3.26 contenente il modello a ostacoli e l'implementazione degli algoritmi FastBroadcast e ROFF.

## Autore
Jordan Gottardo, jordan.gottardo@studenti.unipd.it
