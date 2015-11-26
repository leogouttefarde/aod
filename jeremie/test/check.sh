#!/bin/sh

if [ $# -ge 2 ]; then

	make binary

	PATCH=tmp_patch
	PATCHED=tmp_patched

	./bin/computePatchOpt "$1" "$2" -v > $PATCH
	./bin/applyPatch $PATCH "$1" > $PATCHED

	res=$(diff $PATCHED "$2")

	if [[ $? != 0 ]]; then
		echo "Erreur à la vérification du patch"

	elif [[ $CMD ]]; then
		echo "Patch invalide !"

	else
		echo "Patch valide !"
	fi;

	rm -f $PATCH $PATCHED

else
	echo "Usage :" $0 "<originalFile> <patchedFile>"
fi;

