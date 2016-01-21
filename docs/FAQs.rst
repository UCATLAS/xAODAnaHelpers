.. _faq:

xAH FAQ
=======

This is a list of Frequently Asked Questions about |xAH| and RootCore. Feel free to suggest new entries!

How do I...
-----------

... submit a grid (prun) job?
   Start with a minimal environment::

       lsetup fax
       lsetup panda
       rcSetup

   and a minimal configuration script::

        from xAH_config import xAH_config
        c = xAH_config()
        c.setalg("BasicEventSelection", {"m_name": "test", "m_useMetaData": False})

   Then we can submit a job::

        xAH_run.py --files "user.lgagnon.370150.Gtt.DAOD_SUSY10.e4049_s2608_r6765_r6282_p2411_tag_10_v1_output_xAOD.root" \
        --config=test.py --inputDQ2 prun --optGridMergeOutput=1 \
        --optGridNFilesPerJob=1.0 --optGridOutputSampleName=user.gstark.test

... submit ``xAH_run`` jobs with production privileges?
   You can use ``--optSubmitFlags="--official"`` or ``--optOfficial=1`` (?)::

       xAH_run.py --files MultijetAlgo/scripts/grid_samples_EXOT1_data.txt --inputList \
       --config MultijetAlgo/scripts/config_MultijetAlgo.py -f --inputDQ2 prun \
    --optGridOutputSampleName="group.phys-exotics.%in:name[1]%.%in:name[2]%.%in:name[3]%.v0.1_20150921/" \
    --optSubmitFlags="--official"

