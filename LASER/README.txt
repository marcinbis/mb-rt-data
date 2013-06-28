Projekt aplikacji sterującej laserem dla systemu GNU/Linux.
Marcin Bis - marcin@bis.org.pl
Program jest jednym z przykładów powstającej książki: "Real-Time Linux w zasosowaniach przemysłowych".

Specyfikacja.

Laser (dioda laserowa) jest sterowana impulsami PWM. W przeciwieństwie do zwykłego LED-a, który może być wysterowany sygałem PWM o częstotliwościach rzędu megaherzów, dioda laserowa wymaga pewnego czasu na ustabilizowanie się stanów kwantowych. Aby uzyskać płynną kontolę mocy lasera, stosuje się specjalistyczne układy, które regulują charakterystykę zasilania na podstawie wejścioweg sygnału PWM.

Taki laser może być stosowany w urządzeniach medycznych. Ponadto we wszelkiego rodzaju  maszynach wycinarkających
drobne detale, gdzie moc lasera musi być kontrolowana zależnie od wycinanego właśnie elementu detalu.
