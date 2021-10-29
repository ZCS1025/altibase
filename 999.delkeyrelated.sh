git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/qp/include/qcsCubeOne.h' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/qp/include/qcsDAmo.h' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/qp/qcs/qcsCubeOne.cpp' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/qp/qcs/qcsDAmo.cpp' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/qp/qcs/qcsModule.cpp' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/core/acv/acvLicenseMaker.c' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/mm/main/altibase.cpp' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/id/idMakeRev.sh' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/id/idu/iduCheckLicense.cpp' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch src/id/include/iduCheckLicense.h' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch altibase_home/conf/license ' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch tool/license/printkey.cpp' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch tool/license/makekey.cpp' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch oem_copy.sh' --prune-empty -- --all
git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch oem_env' --prune-empty -- --all
