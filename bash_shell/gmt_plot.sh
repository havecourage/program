psbasemap -R91/122/19/41 -JM12 -B2f0.5WSEN -X1.0i -Y4i -K -P -U > tectonic1.ps
pscoast -JM12 -R91/122/19/41 -G50/50/50 -S150/150/150 -W1 -Df -V -O -K -Na -Ia >> tectonic1.ps

psxy site.txt -JM -R91/122/19/41 -Si0.8 -Gred -O>> tectonic1.ps


psbasemap -R91/122/19/41 -JM12 -B2f0.5WSEN -X1.0i -Y4i -K -P -U > tectonic.ps
pscoast -JM12 -R91/122/19/41 -G50/50/50 -S150/150/150 -W1 -Df -V -O -K -Na -Ia >> tectonic.ps

psxy 1.txt -JM -R91/122/19/41 -Sa2 -Gred -O >> tectonic.ps