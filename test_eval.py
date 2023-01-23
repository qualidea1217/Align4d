from eval import WDER, get_speaker_mapping, get_hyp_aligned_refSpearkerLables
from readAlignment import read_alignment, read_orignal_file


orignal_file = "data/Bdb001_all_tokens.csv"
aligned_file = "data/Bdb001_all_tokens_output.csv"

ref_tokens, ref_spks, rev_tokens, rev_spks =read_orignal_file(orignal_file)
# rev_tokens is our hyp/target sequence here

hyp2ref_align, aligned_sequences = read_alignment(aligned_file)

spk_set = set(rev_spks)
print(spk_set)

speaker_alignment = get_speaker_mapping(rev_spks, aligned_sequences)
print(speaker_alignment)

print(WDER(rev_spks, aligned_sequences))



