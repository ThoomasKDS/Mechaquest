Rapport de Debugging - Fonction algo_attaque

1. Contexte:
    Lors des tests unitaires, une erreur a été découvert et le programme c'est arrêté avec l'erreur suivant
    ```
    Assertion `def.pv < 100' failed.
    ```
    Ce message signifie les point de vie du défensseur n'a pas diminué après l'attaque.

2. Stratégie de débogage:

    Pour le débogage, j'ai utilisé GDB afin d'observer le comportement du programme:
    ```
    gdb ./test_fonction_algo_attaque
    ```

    Avec un point d'arrêt sur la fonction algo_attaque:
    ```
    break algo_attaque
    ```

    Execution du programme:
    ```
    run
    ```
    Afin de naviguer dans la fonction j'ai utilisé s(step) et n(next) pour ensuite, intercepter les variables important:
    ```
    print pv_new
    print mecha_def->pv
    ```
    Et pour vérifier la pile d'appels j'utilise:
    ```
    backtrace
    ```
3. Observations:

    On peut voir que pv_new a bien été calculé. Cependant, mecha_def->pv n'a pas été modifié. On semble donc qu'une insertion a été oublié pour mettre à jour la valeur des PV du défenseur.

4. Conclusion:
    Il manque l'insertion suivante à la fin de la fonction algo_attaque:
    ```
    mecha_def->pv = pv_new;
    ```