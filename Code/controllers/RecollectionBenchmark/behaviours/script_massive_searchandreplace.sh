perl -pi -w -e 's/GET_PARAM\((.*)\)/behaviours::BehavioursParameters::getParameter(\1)/g' *.cpp
