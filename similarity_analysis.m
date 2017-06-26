% this script analyze the similarities in result text file
sim_file = fopen('bestsim_scores.txt')
C = textscan(sim_file, '%s %s %f')
sims = C{3}
avg = mean(sims)
min_sim = min(sims)
max_sim = max(sims)
histogram(sims)
title('Color hist similarity distribution')
xlabel('color similarity')
ylabel('frequency')