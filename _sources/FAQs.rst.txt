.. _faq:

xAH FAQ
=======

This is a list of Frequently Asked Questions about |xAH| and analysis software. Feel free to suggest new entries!

How do I...
-----------

... submit a grid (prun) job?
   Start with a minimal environment::

       lsetup panda

   and a minimal configuration script::

        from xAH_config import xAH_config
        c = xAH_config()
        c.algorithm("BasicEventSelection", {"m_name": "test", "m_useMetaData": False})

   Then we can submit a job::

        xAH_run.py --inputRucio --files "user.lgagnon.370150.Gtt.DAOD_SUSY10.e4049_s2608_r6765_r6282_p2411_tag_10_v1_output_xAOD.root" \
        --config=test.py prun --optGridMergeOutput=1 \
        --optGridNFilesPerJob=1.0 --optGridOutputSampleName=user.gstark.test

... submit ``xAH_run`` jobs with production privileges?
   You can use ``--optSubmitFlags="--official"`` or ``--optOfficial=1`` (?)::

       xAH_run.py --files MultijetAlgo/scripts/grid_samples_EXOT1_data.txt --inputList \
       --config MultijetAlgo/scripts/config_MultijetAlgo.py -f --inputDQ2 prun \
    --optGridOutputSampleName="group.phys-exotics.%in:name[1]%.%in:name[2]%.%in:name[3]%.v0.1_20150921/" \
    --optSubmitFlags="--official"

... use ``AnaToolHandle`` for ASG CP tools?
   Unfortunately there's no much documentation out there, so everything written here comes from direct email question to ASG fellows, or looking at the `source code <https://svnweb.cern.ch/trac/atlasoff/browser/Control/AthToolSupport/AsgTools/trunk/AsgTools/AnaToolHandle.h>`_

   1. Make the tool handle as a member of a xAH algorithm (NB: remember to set the **interface** class of the CP tool. Just prepend an \`I\` to the tool type)::

       class MyAlgo : public xAH::Algorithm
       {
         ...
         private:
           ...
           asg::AnaToolHandle<IMyToolType>   m_mytool_handle; //!

       }

   2. In the xAH algorithm initialisation list, call the tool handle constructor. The argument of the constructor must be a string with the tool type and tool name separated by a slash \`/\` . In general, the tool name in the constructor can be just a dummy string, as it can be changed afterwards::

       MyAlgo :: MyAlgo (std::string className) :
         ...
         m_mytool_handle("MyToolType/MyToolName"),
         ...
        {
          ...
        }

   3. In some cases the name of the tool has to be different than the one set in the constructor. E.g., for the efficiency correctors, the tool names must depend on the configuration of the algorithm, which is set only **after** the initialisation list is executed. In such situations, the name of the tool can be modified (typically this would happen in ``EL::initialize()``) with::

       EL::StatusCode BasicEventSelection :: initialize ()
       {
         ...
         m_mytool_handle.make("MyToolType/MyToolNewName");
         ...
       }

   4. In ``EL::initialize()``, set the properties and initialise the tool handle. After ``m_mytool_handle.initialize()`` has been called, it will effectively behave like a pointer to the tool itself::

	EL::StatusCode BasicEventSelection :: initialize ()
        {
          ...
          m_mytool_handle.make("MyToolType/MyToolNewName");
          m_mytool_handle.SetProperty(...);
          m_mytool_handle.initialize();
          ...
        }

   5. In the algorithm, use the tool associated to the handle via calls like ``m_mytool_handle->doStuff()``.

   6. The tool associated to the handle will be automatically destroyed when appropriate. Hence, no need to call ``delete`` anywhere.

   If the same tool (identified by its name) needs to be used in another xAH algorithm downstream, just declare a tool handle member with the same ``IMyToolType``, call its constructor in the initialisation list and (if needed) change its tool name with ``make()``. Then in ``EL::initialize()`` simply call ``m_mytool_handle.initialize()``, without setting any property. It will automagically get the pointer to the correct tool from a registry, and all the tool properties will be preserved from the previous initialisation.

SLC6 vs SLC7
-----------

If you're running into issues with grid submission because of checks for SLC7-compatible machines in `xAH_run.py` preventing you from doing so, then you can either:

- ssh into lxplus SLC7 (``lxplus.cern.ch``)
- run in a containerized SLC7 environment (``setupATLAS -c slc6``)

If you think this message is happening in error, `file an issue <https://github.com/UCATLAS/xAODAnaHelpers/issues/new>`_ giving us the output from the following commands:

- ``lsb_release -d``
- ``printenv | grep _PLATFORM``
