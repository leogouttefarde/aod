

if [ $# -ge 2 ]; then

	PATCH=tmp_patch
	PATCHED=tmp_patched

	./computePatchOpt "$1" "$2" -v > $PATCH
	./applyPatch $PATCH "$1" > $PATCHED

	res=$(diff $PATCHED "$2")

	if [[ $? != 0 ]]; then
		echo "Erreur"
	elif [[ $CMD ]]; then
		echo "Patch invalide"
	else
		echo "Patch valide"
	fi;

	rm -f $PATCH $PATCHED
fi;

