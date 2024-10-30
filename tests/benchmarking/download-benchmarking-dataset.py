import os
import urllib.request
from concurrent.futures import ThreadPoolExecutor
import threading
import time

benchmarking_dataset_path = os.path.abspath(os.sys.argv[1])

# Create the dataset directory if it doesn't exist
if not os.path.exists(benchmarking_dataset_path):
    os.makedirs(benchmarking_dataset_path)

loops = [
    {
        "name": "[Drumbeats (Melodic)] 120 bpm house loop.mp3",
        "url": "https://cdn.freesound.org/previews/392/392464_1676145-hq.mp3",
    },
    {
        "name": "[Drumbeats (Melodic)] 120 bpm ravish loop.mp3",
        "url": "https://cdn.freesound.org/previews/242/242577_1676145-hq.mp3",
    },
    {
        "name": "[Drumloop] 110 BPM Drums 001.mp3",
        "url": "https://cdn.freesound.org/previews/546/546247_1676145-hq.mp3",
    },
    {
        "name": "[Drumloop] 119 bpm edm drum loop 001.mp3",
        "url": "https://cdn.freesound.org/previews/215/215003_1676145-hq.mp3",
    },
    {
        "name": "[Drumloop] 120 bpm edm drum loop 010.mp3",
        "url": "https://cdn.freesound.org/previews/244/244586_1676145-hq.mp3",
    },
    {
        "name": "[Drumloop] 120 bpm edm drum loop 033.mp3",
        "url": "https://cdn.freesound.org/previews/540/540729_1676145-hq.mp3",
    },
    {
        "name": "[Drumloop] 120 bpm edm perc loop 017.mp3",
        "url": "https://cdn.freesound.org/previews/254/254865_1676145-hq.mp3",
    },
    {
        "name": "[Drumloop] 130 bpm edm drum loop 001.mp3",
        "url": "https://cdn.freesound.org/previews/235/235409_1676145-hq.mp3",
    },
    {
        "name": "[SFX] 128 bpm house lead fx.mp3",
        "url": "https://cdn.freesound.org/previews/213/213867_1676145-hq.mp3",
    },
    {
        "name": "85 BPM Lofi Vibes Melody.mp3",
        "url": "https://cdn.freesound.org/previews/621/621183_12574855-hq.mp3",
    },
    {
        "name": "90 bpm C Key Low Bass.mp3",
        "url": "https://cdn.freesound.org/previews/659/659198_1422197-hq.mp3",
    },
    {
        "name": "99bpm tama break.mp3",
        "url": "https://cdn.freesound.org/previews/177/177006_33044-hq.mp3",
    },
    {
        "name": "100 BPM Dm Chords.mp3",
        "url": "https://cdn.freesound.org/previews/523/523899_3173681-hq.mp3",
    },
    {
        "name": "107bpm_808_Trance_beatz.mp3",
        "url": "https://cdn.freesound.org/previews/653/653656_33044-hq.mp3",
    },
    {
        "name": "116bpm-berserk-beatz.mp3",
        "url": "https://cdn.freesound.org/previews/648/648891_33044-hq.mp3",
    },
    {
        "name": "20161229 - Piano Chord Sequence C-Dmin-A7-Dmin-Emin-B7-Emin 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/371/371894_2518-hq.mp3",
    },
    {
        "name": "Acoustic Loop Lucaz Collab 116BPM.mp3",
        "url": "https://cdn.freesound.org/previews/546/546235_2019171-hq.mp3",
    },
    {
        "name": "Always With You 112 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/566/566927_2019171-hq.mp3",
    },
    {
        "name": "Ambient Lofi Melody Loop 75 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629149_12574855-hq.mp3",
    },
    {
        "name": "backside of the moon Elec git loop 1 90bpm.mp3",
        "url": "https://cdn.freesound.org/previews/444/444923_9221003-hq.mp3",
    },
    {
        "name": "Bamboo Flute  Quenacho Jethro Tull style Loop 5 D 132 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/460/460201_197130-hq.mp3",
    },
    {
        "name": "BANGIN' TECHNO BEAT LOOP 130 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/504/504993_10538321-hq.mp3",
    },
    {
        "name": "Bass 130 Bpm.mp3",
        "url": "https://cdn.freesound.org/previews/323/323528_4397472-hq.mp3",
    },
    {
        "name": "Bass loops 001 with drums short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/566/566160_5674468-hq.mp3",
    },
    {
        "name": "Bass loops 024 short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/585/585468_5674468-hq.mp3",
    },
    {
        "name": "Bass loops 025 with drums short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/585/585338_5674468-hq.mp3",
    },
    {
        "name": "Bass loops 027 with drums short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/586/586020_5674468-hq.mp3",
    },
    {
        "name": "Bass loops 037 short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/593/593835_5674468-hq.mp3",
    },
    {
        "name": "Bass loops 068 with drums short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/612/612134_5674468-hq.mp3",
    },
    {
        "name": "Beat Loop 4-4 80BPM.mp3",
        "url": "https://cdn.freesound.org/previews/390/390613_2481301-hq.mp3",
    },
    {
        "name": "Cinematic Kalimba Loopable Am 126 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/466/466570_197130-hq.mp3",
    },
    {
        "name": "compelectro_140bpm.mp3",
        "url": "https://cdn.freesound.org/previews/137/137979_2514850-hq.mp3",
    },
    {
        "name": "CUSTOMIZE YOUR PAN FLUTE LOOP  Cm 134 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/521/521434_197130-hq.mp3",
    },
    {
        "name": "Dark loops 203 piano without melody short loop 60 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/570/570455_5674468-hq.mp3",
    },
    {
        "name": "Dark loops 209 piano with melody efect short loop 60 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/582/582058_5674468-hq.mp3",
    },
    {
        "name": "Dark loops 214  piano with melody short loop 60 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/583/583433_5674468-hq.mp3",
    },
    {
        "name": "Dark loops 220 octave piano short loop 60 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/617/617174_5674468-hq.mp3",
    },
    {
        "name": "DeepHouse Loop - 120 BpM - Dminor - synthesizer beat.mp3",
        "url": "https://cdn.freesound.org/previews/643/643569_13590673-hq.mp3",
    },
    {
        "name": "Disco funk loops 001 simple mix 1 short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/567/567487_5674468-hq.mp3",
    },
    {
        "name": "Disco funk loops 001 remix 3 short loop with drums 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/572/572397_5674468-hq.mp3",
    },
    {
        "name": "Disco funk loops 001 remix 1 short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/567/567881_5674468-hq.mp3",
    },
    {
        "name": "drum loop 135bpm.mp3",
        "url": "https://cdn.freesound.org/previews/577/577574_12702615-hq.mp3",
    },
    {
        "name": "dubstep_drumloop_amped_glitch_140BPM.mp3",
        "url": "https://cdn.freesound.org/previews/147/147524_1735491-hq.mp3",
    },
    {
        "name": "Dusty Lofi Melody Loop 75 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629153_12574855-hq.mp3",
    },
    {
        "name": "Dusty Lofi Melody Loop 88 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629152_12574855-hq.mp3",
    },
    {
        "name": "Dusty Lofi Melody Loop 95 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629151_12574855-hq.mp3",
    },
    {
        "name": "funky breakbeat loop - 1 bar - 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/585/585466_8705222-hq.mp3",
    },
    {
        "name": "glitch_stutter_loop_200BPM.mp3",
        "url": "https://cdn.freesound.org/previews/147/147522_1735491-hq.mp3",
    },
    {
        "name": "Guiro 2....124bpm.mp3",
        "url": "https://cdn.freesound.org/previews/113/113623_649468-hq.mp3",
    },
    {
        "name": "Guitar loops 003 with drums short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/579/579677_5674468-hq.mp3",
    },
    {
        "name": "Guitar loops 001 with drums short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/574/574451_5674468-hq.mp3",
    },
    {
        "name": "Industrial house loop (1) 122 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/217/217993_2687421-hq.mp3",
    },
    {
        "name": "Lofi Drum Loop 70 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629140_12574855-hq.mp3",
    },
    {
        "name": "loop_94bpm_2bars.mp3",
        "url": "https://cdn.freesound.org/previews/66/66317_800997-hq.mp3",
    },
    {
        "name": "MellifluousAnonymous 120bpm Loop YouAreEvil.mp3",
        "url": "https://cdn.freesound.org/previews/458/458538_5981766-hq.mp3",
    },
    {
        "name": "pearly blips 120bpm.mp3",
        "url": "https://cdn.freesound.org/previews/161/161096_33044-hq.mp3",
    },
    {
        "name": "Piano loops 006 octave short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/636/636237_5674468-hq.mp3",
    },
    {
        "name": "Piano loops 020 octave down short loop 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/639/639460_5674468-hq.mp3",
    },
    {
        "name": "Random Reggaeton 100 BPM Drum Loop.mp3",
        "url": "https://cdn.freesound.org/previews/689/689580_10403690-hq.mp3",
    },
    {
        "name": "Rap beat 001 simple mix 03 short loop 90 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/680/680463_5674468-hq.mp3",
    },
    {
        "name": "Rock Pattern Key of E 120 BPM C.mp3",
        "url": "https://cdn.freesound.org/previews/545/545900_2019171-hq.mp3",
    },
    {
        "name": "Sample 128 Bpm.mp3",
        "url": "https://cdn.freesound.org/previews/322/322378_4397472-hq.mp3",
    },
    {
        "name": "Simple Lofi Vinyl E-Piano Loop 95 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629178_12574855-hq.mp3",
    },
    {
        "name": "Spacey Lofi Loop 90 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629161_12574855-hq.mp3",
    },
    {
        "name": "Spacey Lofi Melody Loop 80 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629157_12574855-hq.mp3",
    },
    {
        "name": "SynTomBeat_113bpm.mp3",
        "url": "https://cdn.freesound.org/previews/137/137031_2514850-hq.mp3",
    },
    {
        "name": "Tambourine_Loop_105bpm.mp3",
        "url": "https://cdn.freesound.org/previews/219/219250_170215-hq.mp3",
    },
    {
        "name": "Vinyl Keyboard Loop 73 BPM.mp3",
        "url": "https://cdn.freesound.org/previews/629/629177_12574855-hq.mp3",
    },
    {
        "name": "Voice loop 001 simple mix 1 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/627/627737_5674468-hq.mp3",
    },
    {
        "name": "Voice loop 001 only guitar synth simple mix 120 bpm.mp3",
        "url": "https://cdn.freesound.org/previews/628/628630_5674468-hq.mp3",
    },
]

nonLoops = [
    {
        "name": "TunnelDrone.mp3",
        "url": "https://cdn.freesound.org/previews/260/260999_985466-hq.mp3",
    },
    {
        "name": "moving plant.mp3",
        "url": "https://cdn.freesound.org/previews/185/185368_670687-hq.mp3",
    },
    {
        "name": "Breaking Bones.mp3",
        "url": "https://cdn.freesound.org/previews/523/523250_3248005-hq.mp3",
    },
    {
        "name": "tone beep lower slower.mp3",
        "url": "https://cdn.freesound.org/previews/263/263126_2064400-hq.mp3",
    },
    {
        "name": "Prison Cell Door sound effect.mp3",
        "url": "https://cdn.freesound.org/previews/613/613498_13597948-hq.mp3",
    },
    {
        "name": "Magic.mp3",
        "url": "https://cdn.freesound.org/previews/335/335354_5865517-hq.mp3",
    },
    {
        "name": "Crash.mp3",
        "url": "https://cdn.freesound.org/previews/512/512246_7704891-hq.mp3",
    },
    {
        "name": "fly.aif.mp3",
        "url": "https://cdn.freesound.org/previews/276/276376_1031833-hq.mp3",
    },
    {
        "name": "7-11-14 Wind Chimes.mp3",
        "url": "https://cdn.freesound.org/previews/251/251280_3984671-hq.mp3",
    },
    {
        "name": "Rustling Paper.mp3",
        "url": "https://cdn.freesound.org/previews/449/449127_8895476-hq.mp3",
    },
    {
        "name": "Bow Loading.mp3",
        "url": "https://cdn.freesound.org/previews/536/536067_10912485-hq.mp3",
    },
    {
        "name": "single firework.mp3",
        "url": "https://cdn.freesound.org/previews/422/422079_4844309-hq.mp3",
    },
    {
        "name": "Wood Creak Single V3.mp3",
        "url": "https://cdn.freesound.org/previews/502/502511_4921277-hq.mp3",
    },
    {
        "name": "portal 40 sec.mp3",
        "url": "https://cdn.freesound.org/previews/205/205198_3423984-hq.mp3",
    },
    {
        "name": "Monster Tripod horn.mp3",
        "url": "https://cdn.freesound.org/previews/267/267069_4875045-hq.mp3",
    },
    {
        "name": "Drone_BowedCybmal.mp3",
        "url": "https://cdn.freesound.org/previews/266/266414_4457609-hq.mp3",
    },
    {
        "name": "Success.mp3",
        "url": "https://cdn.freesound.org/previews/341/341578_5440404-hq.mp3",
    },
    {
        "name": "fire ambience, flames, crackles, pops, burning.mp3",
        "url": "https://cdn.freesound.org/previews/508/508110_6657854-hq.mp3",
    },
    {
        "name": "Thunder Clap And Rumble #2.mp3",
        "url": "https://cdn.freesound.org/previews/469/469612_2247456-hq.mp3",
    },
    {
        "name": "Jetski.mp3",
        "url": "https://cdn.freesound.org/previews/212/212441_2449158-hq.mp3",
    },
    {
        "name": "vintage camera flash bulb pops sound design.mp3",
        "url": "https://cdn.freesound.org/previews/595/595044_4784600-hq.mp3",
    },
    {
        "name": "Footsteps_MountainBoots_Gravel_Mono.mp3",
        "url": "https://cdn.freesound.org/previews/556/556002_9250976-hq.mp3",
    },
    {
        "name": "Magic Sound.mp3",
        "url": "https://cdn.freesound.org/previews/449/449984_2812020-hq.mp3",
    },
    {
        "name": "Wood_Creak_ 01.mp3",
        "url": "https://cdn.freesound.org/previews/128/128659_2035780-hq.mp3",
    },
    {
        "name": "Exploding lightbulb 1.mp3",
        "url": "https://cdn.freesound.org/previews/179/179265_2888453-hq.mp3",
    },
    {
        "name": "Interface3.mp3",
        "url": "https://cdn.freesound.org/previews/141/141258_2555977-hq.mp3",
    },
    {
        "name": "Glass: Putting down 1.mp3",
        "url": "https://cdn.freesound.org/previews/471/471914_3248005-hq.mp3",
    },
    {
        "name": "BangingOilTank.mp3",
        "url": "https://cdn.freesound.org/previews/108/108264_179679-hq.mp3",
    },
    {
        "name": "Orchestra Lurking Cue.mp3",
        "url": "https://cdn.freesound.org/previews/431/431330_4146089-hq.mp3",
    },
    {
        "name": "Heatpress squeaking and creaking.mp3",
        "url": "https://cdn.freesound.org/previews/161/161222_544580-hq.mp3",
    },
    {
        "name": "Rock Splash.mp3",
        "url": "https://cdn.freesound.org/previews/583/583348_9395330-hq.mp3",
    },
    {
        "name": "Bathroom Extractor Fan, A.mp3",
        "url": "https://cdn.freesound.org/previews/400/400577_5121236-hq.mp3",
    },
    {
        "name": "Fairy Tale Synth Bells.mp3",
        "url": "https://cdn.freesound.org/previews/532/532518_8466114-hq.mp3",
    },
    {
        "name": "Left Gravel Footstep 2.mp3",
        "url": "https://cdn.freesound.org/previews/384/384871_984733-hq.mp3",
    },
    {
        "name": "Treadmill.mp3",
        "url": "https://cdn.freesound.org/previews/243/243654_2392999-hq.mp3",
    },
    {
        "name": "50 Caliber MG.flac.mp3",
        "url": "https://cdn.freesound.org/previews/183/183656_71257-hq.mp3",
    },
    {
        "name": "Healing Spell.mp3",
        "url": "https://cdn.freesound.org/previews/563/563662_10912485-hq.mp3",
    },
    {
        "name": "Footsteps_Water_Light_006.mp3",
        "url": "https://cdn.freesound.org/previews/204/204033_1653827-hq.mp3",
    },
    {
        "name": "win.mp3",
        "url": "https://cdn.freesound.org/previews/588/588234_8646358-hq.mp3",
    },
    {
        "name": "Pickup.mp3",
        "url": "https://cdn.freesound.org/previews/371/371217_5937039-hq.mp3",
    },
    {
        "name": "chair hydraulic.mp3",
        "url": "https://cdn.freesound.org/previews/180/180480_7037-hq.mp3",
    },
    {
        "name": "Cup set Down.mp3",
        "url": "https://cdn.freesound.org/previews/272/272022_5165013-hq.mp3",
    },
    {
        "name": "cough.mp3",
        "url": "https://cdn.freesound.org/previews/405/405206_6616150-hq.mp3",
    },
    {
        "name": "Cat 1.mp3",
        "url": "https://cdn.freesound.org/previews/476/476485_8985221-hq.mp3",
    },
    {
        "name": "Game Pickup.mp3",
        "url": "https://cdn.freesound.org/previews/698/698768_9616576-hq.mp3",
    },
    {
        "name": "Screechy Toaster Oven.mp3",
        "url": "https://cdn.freesound.org/previews/388/388804_4357924-hq.mp3",
    },
    {
        "name": "Big Cinematic Spinning Sound.mp3",
        "url": "https://cdn.freesound.org/previews/334/334003_2812020-hq.mp3",
    },
    {
        "name": "Pound of Flesh 2.mp3",
        "url": "https://cdn.freesound.org/previews/528/528263_11537497-hq.mp3",
    },
    {
        "name": "Location Crows NR.WAV.mp3",
        "url": "https://cdn.freesound.org/previews/180/180733_1268407-hq.mp3",
    },
    {
        "name": "Tribal.mp3",
        "url": "https://cdn.freesound.org/previews/533/533976_3554998-hq.mp3",
    },
    {
        "name": "infobleep.mp3",
        "url": "https://cdn.freesound.org/previews/198/198414_3581173-hq.mp3",
    },
    {
        "name": "Ping pong ball hit.mp3",
        "url": "https://cdn.freesound.org/previews/269/269718_3094998-hq.mp3",
    },
    {
        "name": "Magic Fire Impact.mp3",
        "url": "https://cdn.freesound.org/previews/562/562413_10912485-hq.mp3",
    },
    {
        "name": "Wood Snare (Sample 3)wav.mp3",
        "url": "https://cdn.freesound.org/previews/173/173087_1954411-hq.mp3",
    },
    {
        "name": "stone_on_stone_ST.mp3",
        "url": "https://cdn.freesound.org/previews/347/347391_6222517-hq.mp3",
    },
    {
        "name": "OpenBag.mp3",
        "url": "https://cdn.freesound.org/previews/467/467604_9659530-hq.mp3",
    },
    {
        "name": "Paper drawn with rustle.mp3",
        "url": "https://cdn.freesound.org/previews/360/360646_1196472-hq.mp3",
    },
    {
        "name": "fairy-twinklemp3.mp3",
        "url": "https://cdn.freesound.org/previews/620/620672_13739568-hq.mp3",
    },
    {
        "name": "Winding Toy 2.mp3",
        "url": "https://cdn.freesound.org/previews/346/346185_6146418-hq.mp3",
    },
    {
        "name": "Metal Squeak Door.mp3",
        "url": "https://cdn.freesound.org/previews/546/546994_3248005-hq.mp3",
    },
    {
        "name": "Wood Creak.mp3",
        "url": "https://cdn.freesound.org/previews/346/346140_1096807-hq.mp3",
    },
    {
        "name": "flute2 katarina rose.mp3",
        "url": "https://cdn.freesound.org/previews/330/330725_5781159-hq.mp3",
    },
    {
        "name": "Zombie Pack(Clean Record).mp3",
        "url": "https://cdn.freesound.org/previews/393/393749_5229888-hq.mp3",
    },
    {
        "name": "bottle blow processed.mp3",
        "url": "https://cdn.freesound.org/previews/331/331399_5781428-hq.mp3",
    },
    {
        "name": "Dark_Emptiness_003.mp3",
        "url": "https://cdn.freesound.org/previews/504/504704_3905081-hq.mp3",
    },
    {
        "name": "Rock_Movement_Friction_01.mp3",
        "url": "https://cdn.freesound.org/previews/473/473575_9250976-hq.mp3",
    },
    {
        "name": "Rock_Movement_Friction_02.mp3",
        "url": "https://cdn.freesound.org/previews/473/473582_9250976-hq.mp3",
    },
    {
        "name": "Air_PowerTool.mp3",
        "url": "https://cdn.freesound.org/previews/91/91910_1512131-hq.mp3",
    },
    {
        "name": "G38-15-Perfect Horse Whinny.mp3",
        "url": "https://cdn.freesound.org/previews/437/437110_2524442-hq.mp3",
    },
    {
        "name": "Wind Chime.mp3",
        "url": "https://cdn.freesound.org/previews/248/248026_4361022-hq.mp3",
    }
]

lock = threading.Lock()
success = True
num_to_download = 0
num_downloads = 0


def download_file(path, mp3):
    global success
    global num_to_download
    global num_downloads
    loop_path = os.path.join(path, mp3['name'])
    if not os.path.exists(loop_path):
        with lock:
            num_to_download += 1
        try:
            with open(loop_path, 'wb') as file:
                response = urllib.request.urlopen(mp3['url'])
                data = response.read()
                file.write(data)
                with lock:
                    num_downloads += 1
        except Exception as e:
            print(f"Error downloading {mp3['name']}: {e}")
            with lock:
                success = False


loop_path = os.path.join(benchmarking_dataset_path, 'loops')
nonLoop_path = os.path.join(benchmarking_dataset_path, 'non-loops')
if not os.path.exists(loop_path):
    os.makedirs(loop_path)
if not os.path.exists(nonLoop_path):
    os.makedirs(nonLoop_path)

start = time.time()

with ThreadPoolExecutor(max_workers=5) as executor:
    futures = [executor.submit(download_file, loop_path, loop)
               for loop in loops]
    futures += [executor.submit(download_file, nonLoop_path, nonLoop)
                for nonLoop in nonLoops]

print("Downloading benchmarking dataset...")

# Optional: Wait for all downloads to complete (not strictly necessary as executor will do this)
for future in futures:
    future.result()

elapsed = time.time() - start

if num_to_download == 0:
    print("Benchmarking dataset already up-to-date.")
else:
    print("Downloaded " + str(num_downloads) + " of " + str(num_to_download) +
          " loops in " + str(round(elapsed, 1)) + " seconds.")
    if not success:
        print("ERROR: Some loops failed to download. Benchmarking won't be comparable with previous results.")
        exit(1)
