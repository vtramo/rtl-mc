#!/usr/bin/env bash

geogebra_jar=/home/vincenzo/GeoGebra-Linux-Portable-5-2-881-2/geogebra/geogebra.jar
geogebra_java="java -Xms32m -Xmx1024m -Dsun.jnu.encoding=UTF-8 -jar ${geogebra_jar}"

save_geogebra_file_as_pdf() {
  geogebra_file=$1
  $geogebra_java "$geogebra_file.ggb" --export="$geogebra_file.pdf" >/dev/null
}