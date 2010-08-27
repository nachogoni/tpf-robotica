perl -pi -w -e 's/GET_PARAM\((.*)\)/behaviours::BehavioursParameters::getParameter(\1)/g' *.cpp
#perl -pi -w -e 's/([A-Z]+(_[A-Z]+)+)/GET_PARAM(\1)/g' *.cpp
