perl -pi -w -e 's/([A-Z]+(_[A-Z]+)+)/GET_PARAM(\1)/g' *.cpp
